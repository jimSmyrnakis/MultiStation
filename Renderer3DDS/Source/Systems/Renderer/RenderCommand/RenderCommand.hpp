#pragma once

#include <Defs.hpp>
#include "../Materials/Material.hpp"
#include "../GraphicsApi/VABuffer/VABuffer.hpp"
#include "../GraphicsApi/FBuffer/FBuffer.hpp"
#include "../GraphicsApi/Texture/Texture2DResolution.hpp"
#include <glm/glm.hpp>

namespace Game{
    class RenderCommand{
        public:
            RenderCommand(const Texture2DResolution& maxResolution);
            RenderCommand(const RenderCommand& copy) = delete;
            RenderCommand(RenderCommand&& move) = delete;

            RenderCommand& operator=(const RenderCommand& copy) = delete;
            RenderCommand& operator=(RenderCommand&& move) = delete;
            
            void Draw(Material& material , const VABuffer& va);

            void FinalDraw(void); // draw to the back buffer of the default frame buffer 

            void ClearColor(const glm::vec4& rgba );

            void ClearDepth(float clampValue);

            void Clear(bool ClearDepth = true);

            void ClearBackground(void) ;

            void ViewPort(const glm::vec4& viewport);

        private:
            glm::vec4           m_ClearColor    ;
            float               m_ClearDepth    ;
            glm::vec4           m_ViewPort      ;
            FBuffer*            m_FrameBuffer   ;

    };
}