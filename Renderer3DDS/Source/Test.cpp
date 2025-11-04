#include <Test.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_STATIC
#include <GLFW/glfw3.h>
void Test(void) {
	printf("This is a test function.\n");

	// Initialize GLFW
	if (!glfwInit()) {
		printf("Failed to initialize GLFW\n");
		return;
	}

	printf("GLFW initialized successfully.\n");

	// Set up GLFW window hints (optional)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow Forward Compatibility on OpenGL
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	printf("GLFW window hints set.\n");

	// Create a GLFW window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Test Window", NULL, NULL);
	if (!window) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return;
	}

	//Get Window frame buffer width , height
	int FrameBufferWidth, FrameBufferHeight;
	glfwGetFramebufferSize(window, &FrameBufferWidth, &FrameBufferHeight);

	//make window context current
	glfwMakeContextCurrent(window);

	//allow modern extension features
	glewExperimental = GL_TRUE;

	// initiallize glew
	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize GLEW\n");
		glfwDestroyWindow(window);
		glfwTerminate();

		return;
	}

	// Set the viewport size

	glViewport(0, 0, FrameBufferWidth, FrameBufferHeight);

	printf("GLFW window created successfully.\n");

	// now loop while window is not closed
	while (!glfwWindowShouldClose(window)) {
		// Poll for and process events
		glfwPollEvents();
		// Clear the color buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// Swap front and back buffers
		glfwSwapBuffers(window);
	}

	


}