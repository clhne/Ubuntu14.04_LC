#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <vector>
#include <fstream>
#include "utils.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#include <unistd.h>
#endif

/**
* get timestamp(unit: nsec)
*/
uint64_t get_timestamp() {
  uint64_t result = 0;
#ifdef _WIN32
  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);
  result = counter.QuadPart;
#else
  timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  result = (uint64_t)ts.tv_sec * 1000000000LL + (uint64_t)ts.tv_nsec;
#endif
  return result;
}

/**
* get duration(unit: sec)
*/
double get_duration(uint64_t timstamp) {
#ifdef _WIN32
  LARGE_INTEGER result;
  QueryPerformanceFrequency(&result);
  uint64_t freq = result.QuadPart;
#else
  uint64_t freq = 1000000000LL;
#endif
  return (double)(get_timestamp() - timstamp) / freq;
}

/**
* convert float to half
*/
unsigned short float2half(const float src) {
  unsigned short dst;
  uint32_t x = *((uint32_t *)&src);
  uint16_t *h = (uint16_t *)&dst;
  if ( (x & 0x7FFFFFFFu) == 0 ) {  // Signed zero
    *h = (uint16_t) (x >> 16);  // Return the signed zero
  } else { // Not zero
    uint32_t xs = x & 0x80000000u;  // Pick off sign bit
    uint32_t xe = x & 0x7F800000u;  // Pick off exponent bits
    uint32_t xm = x & 0x007FFFFFu;  // Pick off mantissa bits
    if ( xe == 0 ) {  // Denormal will underflow, return a signed zero
      *h = (uint16_t) (xs >> 16);
    } else if ( xe == 0x7F800000u ) {  // Inf or NaN (all the exponent bits are set)
      if ( xm == 0 ) { // If mantissa is zero ...
        *h = (uint16_t) ((xs >> 16) | 0x7C00u); // Signed Inf
      } else {
        *h = (uint16_t) 0xFE00u; // NaN, only 1st mantissa bit set
      }
    } else { // Normalized number
      uint16_t he, hm, hs = (uint16_t) (xs >> 16); // Sign bit
      int hes = ((int)(xe >> 23)) - 127 + 15; // Exponent unbias the single, then bias the halfp
      if ( hes >= 0x1F ) {  // Overflow
        *h = (uint16_t) ((xs >> 16) | 0x7C00u); // Signed Inf
      } else if ( hes <= 0 ) {  // Underflow
        if ( (14 - hes) > 24 ) {  // Mantissa shifted all the way off & no rounding possibility
          hm = (uint16_t) 0u;  // Set mantissa to zero
        } else {
          xm |= 0x00800000u;  // Add the hidden leading bit
          hm = (uint16_t) (xm >> (14 - hes)); // Mantissa
          if( (xm >> (13 - hes)) & 0x00000001u ) // Check for rounding
            hm += (uint16_t) 1u; // Round, might overflow into exp bit, but this is OK
        }
        *h = (hs | hm); // Combine sign bit and mantissa bits, biased exponent is zero
      } else {
        he = (uint16_t) (hes << 10); // Exponent
        hm = (uint16_t) (xm >> 13); // Mantissa
        if ( xm & 0x00001000u ) // Check for rounding
          *h = (hs | he | hm) + (uint16_t) 1u; // Round, might overflow to inf, this is OK
        else
          *h = (hs | he | hm);  // No rounding
      }
    }
  }
  return dst;
}

/**
* convert half to float
*/
float half2float(const unsigned short src) {
  float dst;
  uint16_t h = *((uint16_t *)&src);
  uint32_t *x = (uint32_t *)&dst;
  if( (h & 0x7FFFu) == 0 ) {  // Signed zero
    *x = ((uint32_t) h) << 16;  // Return the signed zero
  } else { // Not zero
    uint32_t xs, xe, xm, xes;
    uint16_t hs = h & 0x8000u;  // Pick off sign bit
    uint16_t he = h & 0x7C00u;  // Pick off exponent bits
    uint16_t hm = h & 0x03FFu;  // Pick off mantissa bits
    if( he == 0 ) {  // Denormal will convert to normalized
      int e = -1; // The following loop figures out how much extra to adjust the exponent
      do {
        e++;
        hm <<= 1;
      } while( (hm & 0x0400u) == 0 ); // Shift until leading bit overflows into exponent bit
      xs = ((uint32_t) hs) << 16; // Sign bit
      xes = ((int32_t) (he >> 10)) - 15 + 127 - e; // Exponent unbias the halfp, then bias the single
      xe = (uint32_t) (xes << 23); // Exponent
      xm = ((uint32_t) (hm & 0x03FFu)) << 13; // Mantissa
      *x = (xs | xe | xm); // Combine sign bit, exponent bits, and mantissa bits
    } else if( he == 0x7C00u ) {  // Inf or NaN (all the exponent bits are set)
      if( hm == 0 ) { // If mantissa is zero ...
        *x = (((uint32_t) hs) << 16) | ((uint32_t) 0x7F800000u); // Signed Inf
      } else {
        *x = (uint32_t) 0xFFC00000u; // NaN, only 1st mantissa bit set
      }
    } else { // Normalized number
      xs = ((uint32_t) hs) << 16; // Sign bit
      xes = ((int32_t) (he >> 10)) - 15 + 127; // Exponent unbias the halfp, then bias the single
      xe = (uint32_t) (xes << 23); // Exponent
      xm = ((uint32_t) hm) << 13; // Mantissa
      *x = (xs | xe | xm); // Combine sign bit, exponent bits, and mantissa bits
    }
  }
  return dst;
}

struct BitmapMagic {
  unsigned char magic[2];
};

struct BitmapHeader {
  unsigned int fileSize; /* brief Total size of the bitmap in bytes. */
  unsigned short creator1; /* brief Reserved field which can be application defined. */
  unsigned short creator2; /* brief Reserved field which can be application defined. */
  unsigned int offset; /* brief Offset in bytes to the beginning of the image data block. */
};

struct BitmapInformationHeader {
  unsigned int size; /* Size of the information headers in bytes. */
  int width; /* Width of the image. */
  int height; /* Height of the image. */
  unsigned short numberOfColorPlanes; /* The number of colour planes. The only legal value is 1. */
  unsigned short bitsPerPixel; /* Number of bits per pixel in the image. */
  unsigned int compressionType; /* Compression type. Use 0 for uncompressed. */
  unsigned int rawBitmapSize; /* Size of the image data including padding (does not include the size of the headers). */
  int horizontalResolution; /* Resolution is in pixels per meter. */
  int verticalResolution; /* Resolution is in pixels per meter. */
  unsigned int numberOfColors; /* Number of colours in the image, can be left as 0. */
  unsigned int numberOfImportantColors; /* Generally ignored by applications. */
};

/**
* read gray from bitmap file
*/
int read_gray_from_bitmap(char* path, unsigned char **gray, int* const width, int* const height) {
  /* Try and open the file for reading. */
  std::ifstream file(path, std::ios::in | std::ios::binary);
  if(!file.is_open()) {
    return -1;
  }

  /*
  * Read and check the headers to make sure we support the type of bitmap passed in.
  */
  struct BitmapMagic magic;
  struct BitmapHeader header;
  struct BitmapInformationHeader informationHeader;

  if (file.read((char*)&magic, sizeof(magic)).bad() || magic.magic[0] != 0x42 || magic.magic[1] != 0x4d) {
    /* Not a valid BMP file header */
    printf("%s is not a valid bmp file\n", path);
    if (file.is_open()) {
      file.close();
    }
    return -2;
  }

  /* 54 is the standard size of a bitmap header. */
  if (file.read((char*)&header, sizeof(header)).bad() || header.offset != 54) {
    /* Not a supported BMP format */
    printf("%s is not a supported bmp file\n", path);
    if (file.is_open()) {
      file.close();
    }
    return -3;
  }

  if (file.read((char*)&informationHeader, sizeof(informationHeader)).bad() || informationHeader.compressionType != 0 || informationHeader.bitsPerPixel != 24) {
    /* We only support uncompressed 24-bits per pixel */
    printf("%s is not a uncompressed 24-bits per pixel bmp file\n", path);
    if (file.is_open()) {
      file.close();
    }
    return -4;
  }

  int row_delta;
  int first_row;
  int after_last_row;
  if (informationHeader.height > 0) {
    /* The image is stored upside down in memory */
    row_delta = -1;
    first_row = informationHeader.height - 1;
    after_last_row = -1;
  } else {
    informationHeader.height = -informationHeader.height;
    row_delta = 1;
    first_row = 0;
    after_last_row = informationHeader.height;
  }

  /* Calculate the paddle of the image to skip it when reading the buffer. */
  int paddedWidth = informationHeader.width * 3;
  while((paddedWidth % 4) != 0) {
    paddedWidth++;
  }

  /* 24-bits per pixel means 3 bytes of data per pixel. */
  int size = paddedWidth * informationHeader.height;
  *gray = new unsigned char[informationHeader.width * informationHeader.height]; // gray buffer
  unsigned char* readBuffer = new unsigned char[size];

  /* Try to read in the image data. */
  if (file.read((char*)readBuffer, size).bad()) {
    printf("error reading data of image file %s\n", path);
    if (file.is_open()) {
      file.close();
    }
    if (readBuffer != NULL) {
      delete [] readBuffer;
    }
    return -5;
  }

  int readBufferIndex = 0;
  /* Loop throught the image data and store it at the output data location. */
  for (int y = first_row; y != after_last_row; y += row_delta) {
    for (int x = 0; x < informationHeader.width; x++) {
      /* The pixels lie in BGR order, we need to resort them into RGB */
      float R = readBuffer[readBufferIndex + 2];
      float G = readBuffer[readBufferIndex + 1];
      float B = readBuffer[readBufferIndex + 0];
      (*gray)[y * informationHeader.width + x] = (unsigned char)(0.2989f * R + 0.5870f * G + 0.1141f * B);
      readBufferIndex += 3;
    }
    /* Skip padding. */
    readBufferIndex += paddedWidth - (informationHeader.width * 3);
  }

  *width  = informationHeader.width;
  *height = informationHeader.height;

  if (file.is_open()) {
    file.close();
  }
  if (readBuffer != NULL) {
    delete [] readBuffer;
  }

  return 0;
}

/**
* write gray data to bitmap file
*/
int write_gray_to_bitmap(char* path, const unsigned char* gray, int width, int height) {
  /* Try and open the file for writing. */
  std::fstream file(path, std::ios::out | std::ios::binary);
  if (!file.is_open()) {
    return -1;
  }

  /* Magic header bits come from the bitmap specification. */
  const struct BitmapMagic magic = { {0x42, 0x4d} };
  struct BitmapHeader header;
  struct BitmapInformationHeader informationHeader;

  /*
   * Each row of the data must be padded to a multiple of 4 bytes according to the bitmap specification.
   * This method uses three bytes per pixel (hence the width * 3).
   * Then we increase the padding until it is divisible by 4.
   */
  int paddedWidth = width * 3;
  while ((paddedWidth % 4) != 0) {
    paddedWidth++;
  }

  /* Setup the bitmap header. */
  header.fileSize = sizeof(magic) + sizeof(header) + sizeof(informationHeader) + paddedWidth * height;
  header.creator1 = 0;
  header.creator2 = 0;
  header.offset = sizeof(magic) + sizeof(header) + sizeof(informationHeader);

  /* Setup the bitmap information header. */
  informationHeader.size = sizeof(informationHeader);
  informationHeader.width = width;
  informationHeader.height = height;
  informationHeader.numberOfColorPlanes = 1;
  informationHeader.bitsPerPixel = 24;
  informationHeader.compressionType = 0;
  informationHeader.rawBitmapSize = paddedWidth * height;
  informationHeader.horizontalResolution = 2835;
  informationHeader.verticalResolution = 2835;
  informationHeader.numberOfColors = 0;
  informationHeader.numberOfImportantColors = 0;

  /* Try to write the header data. */
  if (file.write((char*)&magic, sizeof(magic)).bad() ||
    file.write((char*)&header, sizeof(header)).bad() ||
    file.write((char*)&informationHeader, sizeof(informationHeader)).bad()) {
    printf("write bitmap header to %s failed\n", path);
    if (file.is_open()) {
      file.close();
    }
    return -2;
  }

  for (int y = height - 1; y >= 0; y--) {
    for (int x = 0; x < width; x++) {
      /* The pixels lie in RGB order in memory, we need to store them in BGR order. */
      unsigned char RGB[3];
      RGB[2] = gray[y * informationHeader.width + x];
      RGB[1] = RGB[2];
      RGB[0] = RGB[2];
      if(file.write((char*)RGB, 3).bad()) {
        if (file.is_open()) {
          file.close();
        }
        return -3;
      }
    }
    /*
    * At the end of the row, write out blank bytes to ensure the row length is
    * a multiple of 4 bytes (part of the bitmap specification).
    */
    for (int x = width * 3; x < paddedWidth; x++) {
      char b = 0;
      if(file.write(&b, 1).bad()) {
        if (file.is_open()) {
          file.close();
        }
        return -3;
      }
    }
  }

  return 0;
}
