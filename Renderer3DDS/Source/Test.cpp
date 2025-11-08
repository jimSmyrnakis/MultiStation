#include <Test.hpp>
#include <Core.hpp>
#include <string.h>
#include <time.h>
#include <Renderer/GraphicsApi/VBuffer/VBuffer.hpp>
#include <Renderer/GraphicsApi/Shader/Shader.hpp>
#include <Renderer/GraphicsApi/UBuffer/UBuffer.hpp>
#include <Renderer/GraphicsApi/Texture/Texture2D.hpp>
#include <Renderer/GraphicsApi/FBuffer/FBuffer.hpp>
#include <Renderer/GraphicsApi/Init.hpp>
#include <Renderer/GraphicsApi/Draw/Draw.hpp>
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


// Vertex Shader code
static const char* vShader = "                                                \n\
#version 330                                                                  \n\
                                                                              \n\
layout (location = 0) in vec3 pos;											  \n\
layout (location = 1) in vec2 textCord;                                       \n\
out vec2 TexCoord;															  \n\
uniform mat4 model;                                                           \n\
                                                                              \n\
void main()                                                                   \n\
{                                                                             \n\
    TexCoord = textCord.xy;                                                   \n\
    gl_Position = model * vec4(pos, 1.0);									  \n\
}";

// Fragment Shader
static const char* fShader = R"(
#version 330 core

in vec2 TexCoord;         // Από το vertex shader
out vec4 colour;          // Το τελικό χρώμα pixel

uniform sampler2D tex;    // Η 2D texture

void main()
{
    colour = texture(tex, TexCoord);
}
)";
MultiStation::VBuffer* vertexBuffer;
MultiStation::Texture2D* texture;
void CreateShape()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f ,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f ,
		1.0f , 1.0f , 0.0f

	};

	GLfloat texCoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f
	};

	
	MultiStation::VertexLayout layout;
	MultiStation::VertexAttribute attr(MultiStation::ShaderDataType::VEC3F, false);
	MultiStation::VertexAttribute attr2(MultiStation::ShaderDataType::VEC2F, false);
	vertexBuffer = new MultiStation::VBuffer();
	layout.AddAttribute(attr);
	layout.AddAttribute(attr2);
	vertexBuffer->SetLayout(layout);
	vertexBuffer->SetData(vertices, sizeof(vertices), 0);
	vertexBuffer->SetData(texCoords, sizeof(texCoords), 1);
	texture = new MultiStation::Texture2D(0, MultiStation::Texture2DResolution(256, 256, MultiStation::TextureInternalFormat::RGBA8));
	texture->SetUnit(0);

	
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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


	if (MultiStation::InitGraphicsApi(bufferWidth , bufferHeight) == false)
	{
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	CreateShape();
	MultiStation::FShader fshad(fShader);
	MultiStation::VShader vshad(vShader);
	MultiStation::Shader sha(vshad ,fshad ) ;
	glm::mat4 model(1.0f);
	sha.GetUniforms()->RedirectUniformPointerByName("model", &model[0][0]);
	float* mat = (float*)sha.GetUniforms()->GetUniformPointerByName("model");
	
	MultiStation::FBuffer frameBuffer(
		MultiStation::Texture2DResolution(1024, 1024, MultiStation::TextureInternalFormat::RGBA8));
	MultiStation::DrawParams params;
	params.Mode = MultiStation::DrawMode::TRIANGLES;
	params.Count = 6;
	params.Start = 0;
	params.FrameBuffer = &frameBuffer;
	params.ShaderProgram = &sha;
	params.VertexBuffer = vertexBuffer;
	params.DrawToSurface = false;
	MultiStation::PipelineSettings settings;
	settings.BlendEnabled = false;
	settings.DepthTestEnabled = false;
	settings.Cull.CullFaceEnabled = false;

	MultiStation::Texture2D* frameBufTex = frameBuffer.GetColorBuffer(0);

	float timeStep = 0.0f , prevTime = GetTimeSeconds();
	// Loop until window closed
	float radians = 0.0f;
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get + Handle user input events
		glfwPollEvents();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		frameBuffer.ClearColorBuffer(0, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		


		
		
		
		model = glm::mat4(1.0f);
		radians += (2.0f * 3.141592f) * timeStep;
		model = glm::rotate(model, radians, glm::vec3(0.0f, 0.0f, 1.0f));

		params.FrameBuffer = &frameBuffer;
		texture->Bind();
		MultiStation::DrawCommands::Draw(MultiStation::DrawMode::TRIANGLES, params, settings);

		glViewport(0, 0, bufferWidth, bufferHeight);
		params.FrameBuffer = nullptr;
		frameBufTex->Bind();
		MultiStation::DrawCommands::Draw(MultiStation::DrawMode::TRIANGLES, params, settings);
		
		// swap Buffers and draw
		glfwSwapBuffers(mainWindow);

		float curr = GetTimeSeconds();
		timeStep = curr - prevTime;
		prevTime = curr;
	}

	return 0;
}