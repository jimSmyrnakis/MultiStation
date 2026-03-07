#include "../../mspch.hpp"
#include "Renderer3DDS.hpp"
#include <GLAD.hpp>

namespace MultiStation {

    Renderer3DDS::Renderer3DDS(void) noexcept : IMSSystem("Renderer3DDS") {

    }



    // Vertex Shader
    const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}
)";

    // Fragment Shader
    const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)";
    unsigned int vertexShader = 0;
    unsigned int fragmentShader = 0;
    unsigned int shaderProgram = 0;
    // Triangle vertices
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    unsigned int VAO, VBO;
    ImVec2 viewport;

    unsigned int FBO = 0;
    unsigned int colorTexture = 0;
    unsigned int RBO = 0; // depth-stencil (optional αλλά καλό)
    ImVec2 lastViewportSize = { 0, 0 };
	
	/**
		 * @brief Callback that happens once the system is inserted
		 * at the start of the application
		 *
		 */
	void Renderer3DDS::OnAttach(void) noexcept {
        MS_INFO("Renderer on attached ");
        // Compile Vertex Shader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        // Compile Fragment Shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // Shader Program
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);


        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);


        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        // Color texture
        glGenTextures(1, &colorTexture);
        glBindTexture(GL_TEXTURE_2D, colorTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

        // Depth + stencil (important για future)
        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            MS_ASSERT(false, "Frame Buffer failed to complete");
        }
            

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

    void ResizeFramebuffer(int width, int height)
    {
        if (width == 0 || height == 0) return;

        glBindTexture(GL_TEXTURE_2D, colorTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    }


        

        
	

	/**
	 * @brief Called on a single thread the main/game thread before the ImGui Render Update
	 * for drawing the game scene
	 *
	 * @param deltaTime The time step from the last frame
	 *
	 */
	void Renderer3DDS::OnRenderUpdate(float deltaTime) noexcept {
        // Resize if needed
        if (viewport.x != lastViewportSize.x || viewport.y != lastViewportSize.y)
        {
            ResizeFramebuffer((int)viewport.x, (int)viewport.y);
            lastViewportSize = viewport;
        }

        // Bind framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glViewport(0, 0, viewport.x, viewport.y);

        glEnable(GL_DEPTH_TEST);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Back to default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	/**
	 * @brief Called each frame / game loop to Update the Imgui UI Render , such as creating buttons
	 * etc. This one is called only from the main/game thread only after the Render Update
	 *
	 * @param deltaTime The time step from the previus imgui call of the previus frame
	 *
	 */
	void Renderer3DDS::OnEditorUIRender(float deltaTime) noexcept {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
		ImGui::Begin("Viewport");
        viewport = ImGui::GetContentRegionAvail();

        // IMPORTANT: flip UV γιατί OpenGL != ImGui coords
        ImGui::Image((void*)(uintptr_t)colorTexture,
            viewport,
            ImVec2(0, 1),
            ImVec2(1, 0)
        );
		ImGui::End();
        ImGui::PopStyleVar(1);

        ImGui::Begin("Panel1");
        ImVec2 ButtonViewport = ImGui::GetContentRegionAvail();
        ButtonViewport.x = ButtonViewport.x / 2;
        ImGui::Button("Ha Bouton" ,ButtonViewport);
        ImGui::Bullet();
        ImGui::End();

        ImGui::Begin("Panel2");
        ImGui::Button("Ha Bouton", { 60, 20 });
        ImGui::Bullet();
        ImGui::End();
	}



	/**
	 * @brief Called at the end of the application or at removing the system from the application
	 *
	 * \return
	 */
	void Renderer3DDS::OnDetach(void) noexcept {
        MS_INFO("Renderer On Detached");
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteFramebuffers(1, &FBO);
        glDeleteTextures(1, &colorTexture);
        glDeleteRenderbuffers(1, &RBO);
	}

}
