#pragma once

#include <Defs.hpp>
#include "../Format/Format.hpp"
#include "../Texture/Texture2D.hpp"
#include <glm/glm.hpp>

namespace Game{
    
    class FBuffer{

        public:

            FBuffer(
                const Texture2DResolution& maxResolution , 
                const DepthFormat& depthFormat = DepthFormat::DEPTH32 , 
                const u32& ActiveColorBuffers = 1 ,
                alloc       AllocateFunc  = malloc,
                dalloc      FreeFunc = free
            );
            ~FBuffer(void);

            FBuffer(const FBuffer& copy)            = delete;
            FBuffer(FBuffer&& move)                 = delete;
            FBuffer& operator=(const FBuffer& copy) = delete;
            FBuffer& operator=(FBuffer&& move)      = delete;

            
            u32 GetId(void) const;

            u32 GetColorBuffersCount(void) const;

            void Bind(void) const;
            void Unbind(void) const;

            void ClearColorBuffer(const u32& ActiveColorBuffer , const glm::vec4& rgba) const;
            void ClearDepthBuffer(const float& clampValue) const;

            void SetResolution(const u32& width , const u32& height);

            Texture2D* GetColorBuffer(const u32& ActiveColorBuffer) const;

        private:
            void InitBuffers(void) ;

        private: 
            Texture2DResolution     m_MaxResolution;        // The Maximum resolution for the color buffers
            DepthFormat             m_DepthFormat;          // The Format of the Deapth Render Buffer
            glm::vec4               m_ViewPort;             // The Viewport of this frame buffer

            u32                     m_ObjectId;             // The Frame Buffer Id 
            u32                     m_DepthId;              // The Depth Render Buffer Id
            
            Texture2D**             m_ColorBuffers;         // Textures that are used as color buffer attachment's
            u32                     m_ActiveColorBuffers;   // The number of active color buffer attachments
            
            alloc                   m_Malloc;               // Allocation Function
            dalloc                  m_Free ;                // Free memory function 

            bool                    m_BadState;             // 
    };
}