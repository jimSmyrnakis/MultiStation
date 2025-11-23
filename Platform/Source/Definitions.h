#ifndef MULTISTATION_PLATFORM_H
#define MULTISTATION_PLATFORM_H


#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <vector>
#include <string>


#ifdef _MSC_VER
	#define __break_point_ __debugbreak()
#else 
	#define __break_point_ asm("int3")
#endif

#define ASSERT(x , ...) if (!(x)) { fprintf(stderr , __VA_ARGS__ ); __break_point_;}

#ifndef __cplusplus 
#include <stdbool.h>

#endif 

#ifdef _MSC_VER

#define PACKED_STRUCT(decl) \
              __pragma(pack(push, 1)) decl __pragma(pack(pop))
#define breakpoint() __debugbreak()
#else
#define PACKED_STRUCT(decl) \
              decl __attribute__((__packed__))
#define breakpoint() __builtin_trap()


#endif
#define MS_TEST_BITS(val , bitsVal) (((val) & (bitsVal)) == (bitsVal))

//Status
#define MS_OK			((int)0) // for successfull results
#define MS_ASYNC		((int)1) // for result/ answer from asynchronous operations
//Error codes
#define MS_EINVLDARG	((int)-1) // Invalid arguments
#define MS_ENOMEM		((int)-2) // No mmory or space general
#define MS_EOOB			((int)-3) // Out of Bounds
#define MS_EBDFLGS		((int)-4) // bad flags
#define MS_EBDDESCR		((int)-5) // bad descriptor
#define MS_MISSINGCALLBACKS ((int)-6) // Missing callbacks function pointer's to null
#endif 