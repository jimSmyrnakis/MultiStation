#pragma once

#include <Defs.hpp>
#include "../Format/Format.hpp"
#include "../Texture/Texture2D.hpp"
#include <glm/glm.hpp>

namespace MultiStation{
    
    class FBuffer{

        public:

            FBuffer(
                const Texture2DResolution& maxResolution , 
                const DepthFormat& depthFormat = DepthFormat::DEPTH32 , 
                const uint32_t& ActiveColorBuffers = 1 ,
                alloc       AllocateFunc  = malloc,
                dalloc      FreeFunc = free
            );
            ~FBuffer(void);

            FBuffer(const FBuffer& copy)            = delete;
            FBuffer(FBuffer&& move)                 = delete;
            FBuffer& operator=(const FBuffer& copy) = delete;
            FBuffer& operator=(FBuffer&& move)      = delete;

            
            uint32_t GetId(void) const;

            uint32_t GetColorBuffersCount(void) const;

            void Bind(void) const;
            void Unbind(void) const;

            void ClearColorBuffer(const uint32_t& ActiveColorBuffer , const glm::vec4& rgba) const;
            void ClearDepthBuffer(const float& clampValue) const;

            void SetResolution(const uint32_t& width , const uint32_t& height);

            Texture2D* GetColorBuffer(const uint32_t& ActiveColorBuffer) const;

        private:
            void InitBuffers(void) ;

        private: 
            Texture2DResolution     m_MaxResolution;        // The Maximum resolution for the color buffers
            DepthFormat             m_DepthFormat;          // The Format of the Deapth Render Buffer
            glm::vec4               m_ViewPort;             // The Viewport of this frame buffer

            uint32_t                     m_ObjectId;             // The Frame Buffer Id 
            uint32_t                     m_DepthId;              // The Depth Render Buffer Id
            
            Texture2D**             m_ColorBuffers;         // Textures that are used as color buffer attachment's
            uint32_t                     m_ActiveColorBuffers;   // The number of active color buffer attachments
            
            alloc                   m_Malloc;               // Allocation Function
            dalloc                  m_Free ;                // Free memory function 

            bool                    m_BadState;             // 
    };
}