/*   Rising Tide exhibit                                                    */
/*   Copyright (C) 2010 Khairi Reda                                         */
/*   Electronic Visualization Laboratory                                    */
/*   University of Illinois at Chicago                                      */
/*   http://www.evl.uic.edu/                                                */
/*                                                                          */
/*   This is free software; you can redistribute it and/or modify it        */
/*   under the terms of the  GNU General Public License  as published by    */
/*   the  Free Software Foundation;  either version 2 of the License, or    */
/*   (at your option) any later version.                                    */
/*                                                                          */
/*   This program is distributed in the hope that it will be useful, but    */
/*   WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of    */
/*   MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU    */
/*   General Public License for more details.                               */
/* ======================================================================== */

#include <assert.h>
#include <math.h>
#include <fstream>
#include <tiffio.h>
#include "texture_unit.h"

using namespace std;

// LEFT		red
// RIGHT		green
// TOP		blue
// BOTTOM	alpha

#define	iTL		0	
#define	iT		1
#define iTR		2
#define iL		3
#define iM		4
#define iR		5
#define iBL		6
#define iB		7
#define iBR		8

#define RED		0
#define GREEN		1
#define BLUE		2
#define ALPHA		3

TextureUnit::TextureUnit() : loaded(false), width(0), height(0), channels(0)
{
	raw_texture = NULL;
}

TextureUnit::~TextureUnit()
{
	unloadTexture();	
	delete [] raw_texture;
}


void TextureUnit::flipVertically(unsigned char * image, int w, int oh, int channels, int bpc)
{
	int rowLen = w * channels * bpc;
	unsigned char * temp = new unsigned char[ rowLen ];
	int h = oh / 2;
	
	unsigned int offset = 0;
	unsigned int lastRow = w * (oh-1) * channels * bpc;
		
		
	for (int r = 0; r < h; r++)
	{
		unsigned char * R1 = image + offset;
		unsigned char * R2 = image + lastRow - offset;
			
		memcpy( temp, R1, rowLen );
		memcpy( R1, R2, rowLen );
		memcpy( R2, temp, rowLen );
			
		offset += rowLen;
	}
		
	delete [] temp;	
}

GLenum TextureUnit::getGLTextureFormat(int channels)
{
	GLenum textureFormat;
	switch (channels)
	{
	case TEXTURE_RGB:
		textureFormat = GL_RGB;
		break;
	case TEXTURE_RGBA:
		textureFormat = GL_RGBA;
		break;
	case TEXTURE_MONO:
		textureFormat = GL_LUMINANCE;
		break;
	}
	return textureFormat;
}

bool TextureUnit::loadRaw( const char * filename, int w, int h, int channels, int bpc, bool flip )
{
	// open file
	ifstream input;
	input.open( filename, ios_base::binary );
	if (!input.is_open())
	{
		cerr << "Could not open file: " << filename << endl;
		return false;
	}
	
	// determine expected file size
	size_t expectedLen = w * h * channels * bpc;
	size_t actualLen = 0;
	
	input.seekg(0, ios_base::end);
	actualLen = input.tellg();
	input.seekg(0, ios_base::beg);
	
	// make sure actual == expected
	if ( expectedLen != actualLen )
	{
		cerr << "Raw texture file " << filename << " has " << actualLen << " but expected " << expectedLen << endl;
		input.close();
		return false;
	}
	
	// read the actual file length
	unsigned char * raw_texture = new unsigned char[ actualLen ];
	if (raw_texture == NULL)
	{
		cerr << "Could not allocate memory of size: " << actualLen / 1000 << " MB." << endl;
		return false;
	}
	
	input.read( (char*) raw_texture, actualLen );
	input.close();
	
	// create texture
	glGenTextures(1, &texture_number);
	glBindTexture(GL_TEXTURE_2D, texture_number);
	
	// set initial texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// flip vertically?
	if (flip) {
		flipVertically( raw_texture, w, h, channels, bpc );
	}
	
	// upload image
	GLenum textureFormat = getGLTextureFormat( channels );
	
	// update pixel store alignment
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	
	/*
	glTexImage2D( 
		GL_TEXTURE_2D, 0, textureFormat, 
		w, h, 0, 
		textureFormat, 
		bpc == 1 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT,
		raw_texture
	);
	*/
	
	// build texture mipmaps
	
	gluBuild2DMipmaps( GL_TEXTURE_2D, textureFormat, w, h,
		textureFormat, bpc == 1 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT, raw_texture );
	
	// free up memory
	delete [] raw_texture;
	
	loaded = glOK();
	width = w;
	height = h;
	
	return loaded;	

}

bool TextureUnit::readFile(const char * filename, int _channels)
{
	if (loaded) {
		unloadTexture();
	}
	
	SDL_Surface * photo = IMG_Load(filename);
	if (! photo )
	{
		cerr << "Could not load texture: " << filename << ". Reason: " << IMG_GetError() << endl;
		return false;
	}
	
	delete [] raw_texture;
	raw_texture = transform( photo, _channels );
	width = photo->w;
	height = photo->h;
	channels = _channels;
	
	SDL_FreeSurface(photo);
	loaded = true;
	return true;	
	
}

const unsigned char * TextureUnit::getPixel(int r, int c)
{
	assert(raw_texture);
	return raw_texture + r * width * channels + c * channels;
}


bool TextureUnit::loadTexture(const char * filename, int _channels, bool flip, bool mipMap)
{
	if (filename)
	{
		if (!readFile(filename, _channels))
		{
			return false;
		}
	}
	else if (filename == NULL && raw_texture == NULL)
	{
		cerr << "loadTexture: can not load texture; image file has not been read yet." << endl;
		return false;
	}
		
	// create texture
	glGenTextures(1, &texture_number);
	glBindTexture(GL_TEXTURE_2D, texture_number);
	
	// set initial texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipMap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	
		
	// upload image
	GLenum textureFormat = getGLTextureFormat( channels );
	
	// flip image vertically?
	if (flip)
	{
		flipVertically( raw_texture, width, height, channels, 1 );
	}
	
	
	GLint res = 0;
	if (mipMap)
	{
		GLint res = gluBuild2DMipmaps( GL_TEXTURE_2D, textureFormat, width, height,
			textureFormat, GL_UNSIGNED_BYTE, raw_texture );
	}
	else
	{
		glTexImage2D( 
			GL_TEXTURE_2D, 0, textureFormat, 
			width, height, 0, 
			textureFormat, GL_UNSIGNED_BYTE,
			raw_texture
		);
	}
        
        if (res != 0) {
        		cerr << "Could not make mipmap: " <<gluErrorString( res ) << endl;
        }
        
	// free up memory
	delete [] raw_texture;
	raw_texture = NULL;
	
	loaded = glOK();
	return loaded;
}

bool TextureUnit::unloadTexture()
{
	if (loaded)
	{
		glDeleteTextures(1, &texture_number);
		loaded = false;
		width = height = channels = 0;
		return true;
	}
	else
	{
		// nothing to delete
		return false;
	}
	
	delete [] raw_texture;
	raw_texture = NULL;
}

void TextureUnit::bindTexture()
{
	assert(loaded);
	glBindTexture(GL_TEXTURE_2D, texture_number);	
}


int TextureUnit::activateTexture(int & active_texture_index)
{
	assert(loaded);
	
	int c = active_texture_index++;
	
	glActiveTexture( GL_TEXTURE0 + c);
	glBindTexture(GL_TEXTURE_2D, texture_number);
	
	return c;
}

unsigned char * TextureUnit::transform( SDL_Surface * photo, int channels )
{
	int w = photo->w;
	int h = photo->h;
	int bpp = (int) photo->format->BytesPerPixel;
	int rowOffset = channels * w;
		
	// allocate memory
	unsigned char * data = new unsigned char[ w * h * channels ];
	const unsigned char * pixels = (const unsigned char *) (photo->pixels);

	// loop through pixels from top to bottom
	for (int r = 0; r < h; r++)
	{
		unsigned char * row = data + (h - r - 1) * rowOffset ;
		
		for (int c = 0; c < w; c++, row += channels, pixels += bpp)
		{		
			if (channels <= bpp)
			{
				memcpy(row, pixels, channels);
			}
			else if (bpp == 3 && channels == 4)
			{
				memcpy(row, pixels, bpp);
				row[3] = 255;				
			}
			else if (bpp == 1 && channels == 3)
			{
				memset( row, *pixels, 3 ); 
			}
			else if (bpp == 1 && channels == 4)
			{
				memset( row, *pixels, 3 );
				row[3] = 255;
			}
			
#ifdef __APPLE__
			if (channels >= 3)
			{
				// flip R and B. 
				// For some strange reason we have to do that in Mac to get correct colors
				unsigned char t = row[0];
				row[0] = row[2];
				row[2] = t;
			}
#endif
		}
		
	}
	
	return data;
}

TextureUnit * TextureUnit::edge_detect(const char * filename) const
{	
	assert(raw_texture);
	assert(channels == 4);
	
	const int rowOffset = width * channels;
	unsigned char * edged = new unsigned char[ rowOffset * height ];
	
	// zero
	memset(edged, 0, rowOffset * height);
	
	const unsigned char * prevRow = raw_texture;
	const unsigned char * curRow = raw_texture + rowOffset;
	const unsigned char * nextRow = raw_texture + 2 * rowOffset;	
	
	unsigned char * dCurRow = edged + rowOffset;
	
	for (int r = 1; r < height-1; r++)
	{
		int colOffset = 1 * channels;
		for (int c = 1; c < width-1; c++, colOffset += channels)
		{	
			
			const unsigned char * M	= curRow + colOffset;
			unsigned char * m = dCurRow + colOffset;

			m[0] = 0; m[1] = 0; m[2] = 0; m[3] = 255;
			
			// if I'm black, then ignore me
			if (*M != 255) 
			{
				continue;
			}
			
			const unsigned char * L	= M - channels;
			const unsigned char * R	= M + channels;
			
			const unsigned char * T	= prevRow + colOffset;
			const unsigned char * TL	= T - channels;
			const unsigned char * TR	= T + channels;
			
			const unsigned char * B	= nextRow + colOffset;
			const unsigned char * BL	= B - channels;
			const unsigned char * BR	= B + channels;
			
			const unsigned char kernel[9] = {
				*TL, *T, *TR,
				*L, *M, *R,
				*BL, *B, *BR
			};
			
			int hits = 0;
			int diff[9];
			
			for (int i = 0; i < 9; i++)
			{
				diff[i] = (int) (int) *M - kernel[i];
				if (diff[i] == 0) {
					hits++;
				}
			}
			
			if (hits == 9) {
				// no edges
				continue;
			}
			
			/*
			if (diff[iTL] == 0)
			{
				m[RED]		= 255;
				m[BLUE]		= 255;
			}
			*/
			
			
			if ( diff[iT] == 255 )
			{
				m[RED]		= 255;
			}
			
			
			
			/*
			if (diff[iTR] == 0)
			{
				m[GREEN]		= 255;
				m[BLUE]		= 255;
			}
			*/
			
			/*
			if ( diff[iL] == 255 )
			{
				m[RED]		= 255;
			}
			
			if ( diff[iR] == 255 )
			{
				m[GREEN]		= 255;
			}
			*/
			
			
			
			/*
			if (diff[iBL] == 0)
			{
				m[RED]		= 255;
				m[ALPHA]		= 255;
			}
			*/
			
			
			if ( diff[iB] == 255 )
			{
				m[GREEN]		= 255;
			}
			
			
			
			/*
			if (diff[iBR] == 0)
			{
				m[GREEN]		= 255;
				m[ALPHA]		= 255;
			}
			*/
		}
		
		// increments
		prevRow += rowOffset;
		curRow += rowOffset;
		nextRow += rowOffset;
		
		dCurRow += rowOffset;
		
	}
	
	TextureUnit * result = new TextureUnit;
	result->raw_texture = edged;
	result->loaded = false;
	result->width = width;
	result->height = height;
	result->channels = channels;
	
	if (filename != NULL)
	{
		writeTIFF(filename, edged, width, height, channels);
	}
	
	return result;
}

void TextureUnit::merge(const TextureUnit * other, const char * filename)
{
	assert( raw_texture );
	assert( other->raw_texture );
	
	assert( width == other->width );
	assert(height == other->height );
	assert(channels == other->channels );
	assert(channels == 4);
	
	int rowOffset = width * channels;
	
	unsigned char * curRow = raw_texture;
	const unsigned char * otherCurRow = other->raw_texture;
	
	for (int r = 0; r < height; r++)
	{
		for (int c = 0; c < width; c++, curRow += channels, otherCurRow += channels)
		{
			curRow[2] = otherCurRow[0]; 
			curRow[3] = otherCurRow[1];
		}
	}
	
	if (filename)
	{
		writeTIFF(filename, raw_texture, width, height, channels);
	}
}

bool writeTIFF(const char * filename, unsigned char * theImage, int w, int h, int bpp)
{
	
	bool error = false;
	TIFF * image;
	
	if ( (image = TIFFOpen(filename, "w")) == NULL )
	{
		cerr << "Error opening " << filename << " for TIFF write." << endl;
		return false;
	}

	// set fields	
	TIFFSetField(image, TIFFTAG_IMAGEWIDTH, w);
	TIFFSetField(image, TIFFTAG_IMAGELENGTH, h);
	
	if (bpp > 3) {
		// set extra field
		TIFFSetField(image, TIFFTAG_EXTRASAMPLES, 0);
	}
	
	TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, bpp);
	TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField(image, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);

	TIFFSetField(image, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
	TIFFSetField(image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	TIFFSetField(image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

	// determine size of row
	size_t linebytes = bpp * w;

	unsigned char * buffer = theImage + (h-1) * w * bpp;
	for (unsigned int r = 0; r < h; r++)
	{
		if (TIFFWriteScanline(image, buffer, r, 0) < 0) {
			cerr << "Error writing TIFF file at row: " << r << endl;
			error = true;
			break;
		}
		buffer -= linebytes;
	}
	
	TIFFClose(image);
	return !error;
}

