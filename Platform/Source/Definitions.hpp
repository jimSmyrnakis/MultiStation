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