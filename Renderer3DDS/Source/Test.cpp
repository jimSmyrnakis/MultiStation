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
#include <Platform.hpp>
#include <glm/gtc/matrix_transform.hpp>



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
vec4 color1;
vec4 color2;
uniform float blend;
uniform sampler2D tex;    // Η 2D texture
uniform sampler2D tex2;    // Η 2D texture

void main()
{
    color1 = texture(tex, TexCoord);
	color2 = texture(tex2, TexCoord);
	colour = color1 * (1 - blend) + color2 * blend;
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
void CreateShape()
{
	InitTex2();
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
		material.GetTexture(1)->SetTexture({ 32,32,MultiStation::TextureInternalFormat::RGBA8 }, Tex2, MultiStation::TextureExternalFormat::RGBA8);

	}
	
	MultiStation::FBuffer framebuffer({ 4096,4096,MultiStation::TextureInternalFormat::RGBA8 });
	MultiStation::FBuffer framebufferOff({ 1024,1024,MultiStation::TextureInternalFormat::RGBA8 },
		MultiStation::DepthFormat::DEPTH24, 1, MultiStation::FBuffer::Type::OffScreen);
	MultiStation::DrawParams params;
	params.Count = 6;
	params.Start = 0;
	params.FrameBuffer = &framebufferOff;
	params.VertexBuffer = vertexBuffer;
	params.DrawToSurface = true;
	MultiStation::PipelineSettings settings;
	settings.BlendEnabled = false;
	settings.DepthTestEnabled = false;
	settings.Cull.CullFaceEnabled = false;

	*blend = 0;

	float timeStep = 0.0f , prevTime = MultiStation::Time::GetTimeInSeconds();
	// Loop until window closed
	float radians = 0.0f;
	while (!mainWindow->ShouldClose())
	{
		mainWindow->PollEvents();
		framebuffer.ClearColorBuffer(0, { 0.4f,0.4f,0.4f,0.4f });
		framebufferOff.ClearColorBuffer(0, { 0.4f,0.4f,0.4f,0.4f });
		
		
		
		model = glm::mat4(1.0f);
		radians += (2.0f * 3.141592f) * timeStep;
		model = glm::rotate(model, radians, glm::vec3(0.0f, 0.0f, 1.0f));
		(*blend) += timeStep / 3.0f;
		if ((*blend) > 1) (*blend) = 0;

		
		material.Bind();
		MultiStation::Commands::Draw(MultiStation::DrawMode::TRIANGLES, params, settings);
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