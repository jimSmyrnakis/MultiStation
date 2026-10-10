#include "Init.hpp"
#include <GLAD.hpp>
#include <GLFW.hpp>
#include <iostream>
#include <Platform.hpp>




extern "C" {

	__declspec(dllexport)
		unsigned long NvOptimusEnablement = 0x00000001;

	__declspec(dllexport)
		int AmdPowerXpressRequestHighPerformance = 1;
}




namespace MultiStation {

	
	bool InitGraphicsApi(uint32_t ViewportWidth, uint32_t ViewportHeight) {
		// Setup GLFW window properties
		// OpenGL version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		// Core Profile
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// Allow Forward Compatbility
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		//Initialize GLAD
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}
		else {
			std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
			std::cout << "OpenGL Vendor : " << glGetString(GL_VENDOR) << std::endl;
			std::cout << "OpenGL Renderer:" << glGetString(GL_RENDERER) << std::endl;
		}
		

		GLCALL(glViewport(0, 0, ViewportWidth, ViewportHeight));
		InitErrorHandling();
		return true;

	}

}
