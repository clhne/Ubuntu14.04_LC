#include <stdint.h>
#ifdef _WIN32
#define uint64_t unsigned __int64
#endif
// get timestamp(unit: nsec)
uint64_t get_timestamp();
// get duration(unit: sec)
double get_duration(uint64_t timstamp);
// convert float to half
unsigned short float2half(const float src);
// convert half to float
float half2float(const unsigned short src);
// read gray data from bitmap file
int read_gray_from_bitmap(char* path, unsigned char **gray, int* const width, int* const height);
// write gray data to bitmap file
int write_gray_to_bitmap(char* path, const unsigned char* gray, int width, int height);
