#pragma once


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

