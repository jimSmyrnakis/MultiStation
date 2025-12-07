#include "Error.hpp"
#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_STATIC
#include <GLFW/glfw3.h>
#include <iostream>

void GLClearErrors(void) {
	while (glGetError() != GL_NO_ERROR);
}


bool GLLogCall(const char* FunctionName, const char* SourceFile, unsigned int line) {
	GLenum error;
	bool nfound = true;
	while ((error = glGetError()) != GL_NO_ERROR) {
		std::cerr << "Error Code ( " << (int)error <<
			" ) at function \" " << FunctionName <<
			" \" , Source File \" " << SourceFile <<
			" \" at line " << line << std::endl;
		nfound = false;
	}
	return nfound;
}

namespace MultiStation {

	void GLDebugCallback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length,
		const GLchar* message, const void* userParam)
	{
		std::cerr << "OpenGL Debug [" << id << "]: " << message << std::endl;
	}

	void InitErrorHandling() {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugCallback, nullptr);
	}

}