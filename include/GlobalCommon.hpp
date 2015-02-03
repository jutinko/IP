#ifndef GLOBAL_COMMON_HPP_
#define GLOBAL_COMMON_HPP_
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

namespace FLAT
{
	typedef double  spaceUnit;
	typedef double bigSpaceUnit;
	#define PAGE_SIZE 4096

	#define DIMENSION 3 // should be atleast 1
	#define FILE_BUFFER_SIZE 4*PAGE_SIZE
	#define RAW_DATA_HEADER_SIZE 16+(2*DIMENSION*sizeof(spaceUnit))

	#ifdef WIN32
		typedef char int8;
		typedef int  int32;
		typedef unsigned char uint8;
		typedef unsigned short uint16;
		typedef unsigned int  uint32;
		typedef unsigned long long uint64;
		typedef long long int64;
	#else
		typedef char int8;
		typedef int int32;
		typedef uint8_t uint8;
		typedef uint16_t uint16;
		typedef uint32_t  uint32;
		typedef uint64_t uint64;
		typedef int64_t int64;
	#endif
}

#endif
