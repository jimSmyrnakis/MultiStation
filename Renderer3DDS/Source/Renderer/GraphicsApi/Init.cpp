#include "Init.hpp"
#include <GL/glew.h>
#include <glfw/glfw3.h>
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
	bool InitGraphicsApi(uint32_t ViewportWidth, uint32_t ViewportHeight) {
		// Setup GLFW window properties
		// OpenGL version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		// Core Profile
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// Allow Forward Compatbility
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		//Initialize GLEW
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			// Problem: glewInit failed, something is seriously wrong.
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
			ASSERT(0, "Failed to Initialize GLEW !");
			return false;
		}
		else {
			fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
		}
		

		GLCALL(glViewport(0, 0, ViewportWidth, ViewportHeight));
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugCallback, nullptr);
		return true;

	}

}
