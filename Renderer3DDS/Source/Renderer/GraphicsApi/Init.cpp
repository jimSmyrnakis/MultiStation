#include "Init.hpp"
#include <GL/glew.h>
#include <iostream>
namespace MultiStation {
	void GLDebugCallback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length,
		const GLchar* message, const void* userParam)
	{
		std::cerr << "OpenGL Debug [" << id << "]: " << message << std::endl;
	}
	bool InitGraphicsApi(uint32_t ViewportWidth, uint32_t ViewportHeight) {
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
