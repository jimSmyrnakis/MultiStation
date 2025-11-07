#include "Material.hpp"
#include <glad/gl.h>
#include <string.h>





namespace Game{
    Material2D::Material2D(Shader& shader , Resolution Res , alloc allocate , dalloc freemem){
        m_Shader = &shader;
        m_CommonFormat = Res;
        
        if (allocate == nullptr){
            ASSERT(0 , "Allocation Function is nullptr !!!");
            allocate = malloc;
        }

        if (freemem == nullptr){
            ASSERT(0 , "Free memory Function is nullptr !!!");
            freemem = free;
        }

        m_Malloc = allocate;
        m_Free   = freemem;
        

        InitMaterials();

    }

    Material2D::~Material2D(void){
        if (m_Textures)
            m_Free(m_Textures);
        
        if (m_TextureSlots)
            m_Free(m_TextureSlots);
        
        if (m_SamplerNames){
            
            m_Free(m_SamplerNames[0]);
            m_Free(m_SamplerNames);
        }
    }

    void Material2D::UpdateMaterials(void){
        if (m_Count == 0) 
            return;
        if (m_Textures == nullptr){
            
            ASSERT(0 , " No Uniforms Data !!!");
            return;
        }

        for (int i = 0; i < m_Count ; i++){
            u32* slot = (u32*)m_Shader->GetUniforms()->GetUniformPointerByName(m_SamplerNames[i]);
            (*slot) = m_TextureSlots[i];
            m_Textures[i]->SetUnit((*slot));
            m_Textures[i]->Bind();
        }
        
    }
           
    u32 Material2D::GetTextureCount(void) const {return m_Count;}
    
    void Material2D::Bind(void)  {
        if (m_Shader == nullptr){
            ASSERT(0 , "Something is realy wrong !!!");
            return;
        }

        if ( (m_Textures) && (m_Count) ){
            UpdateMaterials();
        }

        m_Shader->Bind();

    }
    
    void Material2D::Unbind(void)  {

        if (m_Shader == nullptr){
            ASSERT(0 , "Something is realy wrong !!!");
            return;
        }

        if ((m_Textures) && (m_Count)){
            for (int i = 0; i < m_Count ; i++){
                m_Textures[i]->Unbind();
            }
        }

        

        m_Shader->Unbind();
        
    }
    
    Shader& Material2D::GetShader(void) {return *m_Shader; }
    const Shader& Material2D::GetShader(void) const { return *m_Shader; }

    TextureExternalFormat GetExternalFormatFromSamplers(ImageFormat format , UniformType type){
        if (type == UniformType::SAMPLER_2D){
            switch(format){
                case ImageFormat::RGB4      :   return  TextureExternalFormat::RGB4         ;
                case ImageFormat::RGB8      :   return  TextureExternalFormat::RGB8         ;
                case ImageFormat::RGB16     :   return  TextureExternalFormat::RGB16        ;
                case ImageFormat::RGB32     :   return  TextureExternalFormat::RGB32        ;

                case ImageFormat::RGBA4     :   return  TextureExternalFormat::RGBA4        ;
                case ImageFormat::RGBA8     :   return  TextureExternalFormat::RGBA8        ;
                case ImageFormat::RGBA16    :   return  TextureExternalFormat::RGBA16       ;
                case ImageFormat::RGBA32    :   return  TextureExternalFormat::RGBA32       ;

                case ImageFormat::RGB16F    :   return  TextureExternalFormat::RGB16F_RAW   ;
                case ImageFormat::RGB32F    :   return  TextureExternalFormat::RGB32F_RAW   ;

                case ImageFormat::RGBA16F   :   return  TextureExternalFormat::RGBA16F_RAW  ;
                case ImageFormat::RGBA32F   :   return  TextureExternalFormat::RGBA32F_RAW  ;

                default                     :   break;

            }
        }

        if (type == UniformType::USAMPLER_2D){
            switch(format){
                case ImageFormat::RGB4      :   return  TextureExternalFormat::RGB4U_RAW         ;
                case ImageFormat::RGB8      :   return  TextureExternalFormat::RGB8U_RAW         ;
                case ImageFormat::RGB16     :   return  TextureExternalFormat::RGB16U_RAW        ;
                case ImageFormat::RGB32     :   return  TextureExternalFormat::RGB32U_RAW        ;

                case ImageFormat::RGBA4     :   return  TextureExternalFormat::RGBA4U_RAW        ;
                case ImageFormat::RGBA8     :   return  TextureExternalFormat::RGBA8U_RAW        ;
                case ImageFormat::RGBA16    :   return  TextureExternalFormat::RGBA16U_RAW       ;
                case ImageFormat::RGBA32    :   return  TextureExternalFormat::RGBA32U_RAW       ;

                case ImageFormat::RGB16F    :   return  TextureExternalFormat::RGB16F_RAW   ;
                case ImageFormat::RGB32F    :   return  TextureExternalFormat::RGB32F_RAW   ;

                case ImageFormat::RGBA16F   :   return  TextureExternalFormat::RGBA16F_RAW  ;
                case ImageFormat::RGBA32F   :   return  TextureExternalFormat::RGBA32F_RAW  ;

                default                     :   break;

            }
        }

        if (type == UniformType::ISAMPLER_2D){
            switch(format){
                case ImageFormat::RGB4      :   return  TextureExternalFormat::RGB4I_RAW         ;
                case ImageFormat::RGB8      :   return  TextureExternalFormat::RGB8I_RAW         ;
                case ImageFormat::RGB16     :   return  TextureExternalFormat::RGB16I_RAW        ;
                case ImageFormat::RGB32     :   return  TextureExternalFormat::RGB32I_RAW        ;

                case ImageFormat::RGBA4     :   return  TextureExternalFormat::RGBA4I_RAW        ;
                case ImageFormat::RGBA8     :   return  TextureExternalFormat::RGBA8I_RAW        ;
                case ImageFormat::RGBA16    :   return  TextureExternalFormat::RGBA16I_RAW       ;
                case ImageFormat::RGBA32    :   return  TextureExternalFormat::RGBA32I_RAW       ;

                case ImageFormat::RGB16F    :   return  TextureExternalFormat::RGB16F_RAW   ;
                case ImageFormat::RGB32F    :   return  TextureExternalFormat::RGB32F_RAW   ;

                case ImageFormat::RGBA16F   :   return  TextureExternalFormat::RGBA16F_RAW  ;
                case ImageFormat::RGBA32F   :   return  TextureExternalFormat::RGBA32F_RAW  ;

                default                     :   break;

            }
        }

        ASSERT(0 , "Undefined sampler or image format ");
        return TextureExternalFormat::RGB16;
    }

    void Material2D::SetTextureData(const char* samplerName , void* image , u32 width , u32 height , ImageFormat format){
        if ((m_Textures == nullptr) || (m_Count == 0)){
            ASSERT(0 , "Error --- No Textures exist's for this material !!!");
            return;
        }

        u32 index = 0;
        while(index < m_Count){
            
            if (strcmp(samplerName, m_SamplerNames[index]) == 0)
                break;

            index++;
            
        }

        if (index >= m_Count){
            ASSERT(0 , "No such a uniform name :( !!!");
            return ;
        }
            
        UniformType type = m_Shader->GetUniforms()->GetUniformTypeByName(m_SamplerNames[index]);
        ASSERT(type == UniformType::SAMPLER_2D , "What not a sampler 2D ? !!!");
        TextureExternalFormat EFormat = GetExternalFormatFromSamplers(format , type);
        ASSERT( EFormat == TextureExternalFormat::RGBA8 , "But hiow ?????");
        m_Textures[index]->SetSubImage(image , 0 , 0 , width , height , EFormat);

    }

 }