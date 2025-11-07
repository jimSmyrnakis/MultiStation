#pragma once

#include <Defs.hpp>
#include "../GraphicsApi/Shader/Shader.hpp"
#include "../GraphicsApi/Texture/Texture2D.hpp"
#include "../GraphicsApi/Texture/Texture2DResolution.hpp"
#include "../GraphicsApi/Format/Format.hpp"
#include "ImageFormat.hpp"

namespace Game{
    
    class Material{
        public:
            virtual ~Material(void) = default;

            virtual void Bind(void)  = 0;
            virtual void Unbind(void)  = 0;

    };
    
    class Material2D : public Material{
        public:
            enum class Format{
                RGB4    ,
                RGB8    ,
                RGB16   ,
                RGBA4   ,
                RGBA8   ,
                RGBA16  
            };
            
            class Resolution{
                public:
                    Resolution(const u32& width = 64 , const u32& height = 64 , const Format& format = Format::RGBA8);

                    void SetFormat(const Format& format);
                    void SetWidth (const u32& width    );
                    void SetHeight(const u32& height   );

                    Format GetFormat(void) const;
                    u32    GetWidth (void) const;
                    u32    GetHeight(void) const;


                private:
                    u32         m_Width ;
                    u32         m_Height;
                    Format      m_Format;
            };
        public:
            Material2D(Shader& shader , Resolution Res , alloc allocate = malloc , dalloc freemem = free);
            //from the shader this class will need to find all sample2D (for moment) uniforms
            //and so the number of textures

            ~Material2D(void);
            //only the textures , texture slots will deleted here the rest is shared data that the Material have not create

            u32 GetTextureCount(void) const;
            //Get the number of texture as the shader program has defined them 

            void Bind(void) override;
            //binds all the things together to set the new material

            void Unbind(void) override;
            //reverse of the bind method

            Shader& GetShader(void);
            const Shader& GetShader(void) const;

            void SetTextureData(const char* samplerName , void* image ,  u32 width , u32 height , ImageFormat format);
            //based on the sampler name (the sampler2D uniform name) sends , transform and store the new image data
        private:
            void UpdateMaterials(void);
            void InitMaterials(void) ; 

        private:
            Resolution                      m_CommonFormat;     // defines the actual resolution of the material
            Shader*                         m_Shader;           // the shader for this material
            u32*                            m_TextureSlots;     // the all texture slots that will be used
            Texture2D**                     m_Textures;         // the textures that will be used
            u32                             m_Count;            // The Number of the textures (defined by the shader sampler uniforms number)
            char**                          m_SamplerNames;     // storing the sampler names for faster searching
            alloc                           m_Malloc;           // the allocation function
            dalloc                          m_Free;             // the free function

    };

}