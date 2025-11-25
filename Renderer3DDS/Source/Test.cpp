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
#include <Renderer/GraphicsApi/Commands/Commands.hpp>
#include <Renderer/Material/Material.hpp>
#include <Renderer/Model/Model.h>
#include <Platform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>


// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;


// Vertex Shader code
static const char* vShader = "                                                \n\
#version 330                                                                  \n\
                                                                              \n\
layout (location = 0) in vec3 pos;											  \n\
layout (location = 1) in vec3 normal;										  \n\
layout (location = 2) in vec2 texCord;										  \n\
out vec2 TexCoord;															  \n\
uniform mat4 model;                                                           \n\
out vec3 Normal;                                                              \n\
void main()                                                                   \n\
{                                                                             \n\
    TexCoord = texCord.xy;                                                   \n\
	Normal = normal;														  \n\
    gl_Position = model * vec4(pos, 1.0);									  \n\
}";

// Fragment Shader
static const char* fShader = R"(
#version 330 core

in vec2 TexCoord;         // Από το vertex shader
in vec3 Normal;			  
out vec4 colour;          // Το τελικό χρώμα pixel
vec4 color1;
vec4 color2;
uniform float blend;
uniform sampler2D tex;    // Η 2D texture
uniform sampler2D tex2;    // Η 2D texture

void main()
{
	float EnhanceColor = 2.5;
	float LowColor = 0.5;
    color1 = texture(tex, TexCoord) * vec4(EnhanceColor,EnhanceColor,EnhanceColor,1);;
	color2 = texture(tex, TexCoord) * vec4(LowColor,LowColor,LowColor,1) ;
	
	colour = vec4(Normal.x,Normal.y,Normal.z,1) *(color1 * (1 - blend)  + color2 * blend);
}
)";



MultiStation::VBuffer* vertexBuffer;
MultiStation::Texture2D* texture;
uint32_t Tex2[32][32];
void InitTex2(void) {
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 32; j++) {
			Tex2[i][j] = 0xFF0000FF;
		}
	}
}
MultiStation::Model g_model;
void CreateShape()
{
	InitTex2();
	
	std::string filepath = std::string("C:\\Users\\User\\Desktop\\Projects\\MultiStation\\Assets\\Models\\Dragon\\Dragon.fbx");
	g_model.LoadModel(filepath);
	
	

	
}

glm::mat4 BuildMVP(float angleDeg, float aspect , float blend)
{
	// Model matrix (rotate + scale)
	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(angleDeg * blend), glm::vec3(1.0f, 1.0f, 1.0f));
	
	// View matrix (απλή κάμερα που κοιτάει το 0,0,0)
	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 80.0f),  // πίσω 3 units
		glm::vec3(0.0f, 0.0f, 0.0f),  // στο origin
		glm::vec3(0.0f, 1.0f, 1.0f)   // up vector
	);

	// Projection matrix
	glm::mat4 projection = glm::perspective(
		glm::radians(80.0f), // FoV
		aspect,              // width / height
		0.1f,                // near plane
		1000.0f              // far plane
	);

	return projection * view * model;
}

int Test(void)
{
	
	MultiStation::Window* mainWindow = new MultiStation::Window();
	// Get Buffer Size information
	int bufferWidth, bufferHeight;
	bufferHeight = mainWindow->GetSurfaceHeight();
	bufferWidth = mainWindow->GetSurfaceWidth();



	if (MultiStation::InitGraphicsApi(bufferWidth , bufferHeight) == false)
	{
		delete mainWindow;
		return 1;
	}

	CreateShape();
	MultiStation::Material material;
	material.SetShader(vShader, fShader);
	glm::mat4 model(1.0f);
	material.GetUniforms()->RedirectUniformPointerByName("model", &model[0][0]);
	float* mat = (float*)material.GetUniforms()->GetUniformPointerByName("model");
	float* blend = (float*)material.GetUniforms()->GetUniformPointerByName("blend");
	if (material.GetTexturesCount() == 2) {
		material.GetTexture(1)->SetTexture(
			{ 32,32,MultiStation::TextureInternalFormat::RGBA8 }, 
			Tex2, MultiStation::TextureExternalFormat::RGBA8);

	}
	
	MultiStation::FBuffer framebuffer({ 4096,4096,MultiStation::TextureInternalFormat::RGBA8 },
		MultiStation::DepthFormat::DEPTH24, 1);
	MultiStation::FBuffer framebufferOff({ 1024,1024,MultiStation::TextureInternalFormat::RGBA8 },
		MultiStation::DepthFormat::DEPTH24, 1, MultiStation::FBuffer::Type::OffScreen);
	MultiStation::DrawParams params;
	params.Count = 6;
	params.Start = 0;
	params.FrameBuffer = &framebuffer;
	params.VertexBuffers = g_model.GetVBuffers();
	params.VBCount = g_model.GetVBuffersCount();

	*blend = 0;
	framebufferOff.ClearDepthBuffer(1.0f);
	float timeStep = 0.0f , prevTime = MultiStation::Time::GetTimeInSeconds();
	// Loop until window closed
	float radians = 0.0f;
	int Direction = 1;
	float testBlend = 0;
	while (!mainWindow->ShouldClose())
	{
		mainWindow->PollEvents();
		framebuffer.ClearColorBuffer(0, { 0.4f,0.4f,0.4f,0.4f });
		framebuffer.ClearDepthBuffer(1);
		framebufferOff.ClearColorBuffer(0, { 0.4f,0.4f,0.4f,0.4f });
		
		
		
		model = BuildMVP(360, 1 , testBlend);
		testBlend += timeStep / 3.0f;
		(*blend) += (Direction) * timeStep / 3.0f;
		if ((*blend) > 1) {
			(*blend) = 1;
			Direction = -1;
			testBlend = 0;
		}
		if ((*blend) < 0) {
			Direction = 1;
			testBlend = 0;
		}

		framebuffer.SetResolution(mainWindow->GetSurfaceWidth(), mainWindow->GetSurfaceHeight());
		material.Bind();
		MultiStation::Commands::Draw(MultiStation::DrawMode::TRIANGLES, params);
		material.Unbind();


		
		// swap Buffers and draw
		mainWindow->SwapBuffers();

		printf("blend : %f\n", (*blend));
		float curr = MultiStation::Time::GetTimeInSeconds();
		timeStep = curr - prevTime;
		prevTime = curr;
	}

	return 0;
}