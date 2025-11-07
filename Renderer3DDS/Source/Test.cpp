#include <Test.hpp>
#include <Core.hpp>
#include <string.h>
#include <time.h>
#include <Renderer/GraphicsApi/VBuffer/VBuffer.hpp>
#include <Renderer/GraphicsApi/Shader/Shader.hpp>
#include <Renderer/GraphicsApi/UBuffer/UBuffer.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

float GetTimeSeconds() {
	using namespace std::chrono;
	static auto start = high_resolution_clock::now();
	auto now = high_resolution_clock::now();
	duration<float> elapsed = now - start;
	return elapsed.count(); // επιστρέφει float σε δευτερόλεπτα (με κλάσματα)
}


// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;
const float toRadians = 3.14159265f / 180.0f;
float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Vertex Shader code
static const char* vShader = "                                                \n\
#version 330                                                                  \n\
                                                                              \n\
layout (location = 0) in vec3 pos;											  \n\
                                                                              \n\
uniform mat4 model;                                                           \n\
                                                                              \n\
void main()                                                                   \n\
{                                                                             \n\
    gl_Position = model * vec4(pos, 1.0);									  \n\
}";

// Fragment Shader
static const char* fShader = "                                                \n\
#version 330                                                                  \n\
                                                                              \n\
out vec4 colour;                                                              \n\
                                                                              \n\
void main()                                                                   \n\
{                                                                             \n\
    colour = vec4(1.0, 0.0, 0.0, 1.0);                                        \n\
}";
MultiStation::VBuffer* vertexBuffer;
void CreateTriangle()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	
	MultiStation::VertexLayout layout;
	MultiStation::VertexAttribute attr(MultiStation::ShaderDataType::VEC3F, false);
	vertexBuffer = new MultiStation::VBuffer();
	layout.AddAttribute(attr);
	vertexBuffer->SetLayout(layout);
	vertexBuffer->SetData(vertices, sizeof(vertices), 0);


	
}



int Test(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}
	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow Forward Compatbility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer Size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	MultiStation::FShader fshad(fShader);
	MultiStation::VShader vshad(vShader);
	MultiStation::Shader sha(vshad ,fshad ) ;
	glm::mat4 model(1.0f);
	sha.GetUniforms()->RedirectUniformPointerByName("model", &model[0][0]);
	float* mat = (float*)sha.GetUniforms()->GetUniformPointerByName("model");
	
	//Com pileShaders();
	
	float timeStep = 0.0f , prevTime = GetTimeSeconds();
	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get + Handle user input events
		glfwPollEvents();
		// Clear window// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		if (direction)
		{
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset)
		{
			direction = !direction;
		}

		curAngle += 0.1f;
		if (curAngle >= 360)
		{
			curAngle -= 360;
		}

		if (direction)
		{
			curSize += 0.001f;
		}
		else {
			curSize -= 0.001f;
		}

		if (curSize >= maxSize || curSize <= minSize)
		{
			sizeDirection = !sizeDirection;
		}


		
		
		model = glm::mat4(1.0f);
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(curSize, curSize, 0.0f));

		//memcpy(mat, &model[0][0], 16 * 4);
		
		sha.Bind();
		vertexBuffer->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		vertexBuffer->Unbind();

		sha.Unbind();

		glfwSwapBuffers(mainWindow);

		float curr = GetTimeSeconds();
		timeStep = curr - prevTime;
		prevTime = curr;
	}

	return 0;
}