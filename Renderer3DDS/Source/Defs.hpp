#pragma once

#include <stdint.h>

#define _CRT_SECURE_NO_WARNINGS
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <vector>
#include <string>


typedef void* (*alloc)(size_t);
typedef void  (*dalloc)(void*);

#ifdef _MSC_VER
    #define __break_point_ __debugbreak()
#else 
    #define __break_point_ asm("int3")
#endif

#define ASSERT(x , ...) if (!(x)) { fprintf(stderr , __VA_ARGS__ ); __break_point_;}

void    GLClearErrors  (void);
bool    GLLogCall(const char* FunctionName , const char* SourceFile , unsigned int line);
#define GLCALL(function) \
        GLClearErrors();\
        (function);\
        ASSERT(GLLogCall(#function , __FILE__ , __LINE__ ) , ":(");
