#pragma once

#include <stdint.h>
#include <stddef.h>
#include <Platform.hpp>
void    GLClearErrors(void);
bool    GLLogCall(const char* FunctionName, const char* SourceFile, unsigned int line);

#define GLCALL(function) \
		GLClearErrors();\
		(function);\
		MS_ASSERT(GLLogCall(#function , __FILE__ , __LINE__ ) , ":(");


namespace MultiStation {

	
	/**
	 * \author Dimitris Smyrnakis
	 * \details Initializes OpenGL error handling by setting up a debug message callback.
	 */
	void InitErrorHandling();

}
