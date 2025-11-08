#pragma once

#include <Defs.hpp>



void    GLClearErrors(void);
bool    GLLogCall(const char* FunctionName, const char* SourceFile, unsigned int line);
#define GLCALL(function) \
        GLClearErrors();\
        (function);\
        ASSERT(GLLogCall(#function , __FILE__ , __LINE__ ) , ":(");

namespace MultiStation {

	bool InitGraphicsApi(uint32_t ViewportWidth , uint32_t ViewportHeight);

}
