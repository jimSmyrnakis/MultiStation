#include "Texture2D.hpp"
#include "../Init.hpp"
#include <GL/glew.h>
namespace MultiStation {
    #define DEFAULT_TEX2D_DIM 1024

    uint32_t DefaultTexture2D[DEFAULT_TEX2D_DIM][DEFAULT_TEX2D_DIM] ;
    static bool isInitiallized = false;
    uint32_t* GetDefaultTexture2DData(uint32_t* width, uint32_t* height) {
        (*width) = (*height) = DEFAULT_TEX2D_DIM;
        if (isInitiallized == false)
            for (uint32_t y = 0; y < DEFAULT_TEX2D_DIM; ++y)
                for (uint32_t x = 0; x < DEFAULT_TEX2D_DIM; ++x)
                    DefaultTexture2D[y][x] = x * y * 1024;
        isInitiallized = true;
        return (uint32_t*)DefaultTexture2D;
    }

    //TODO - Request the maximum number of texture units that can be active at a draw
    uint32_t GetMaxTextureUnits(void) { return 16; } // for moment 16 is sure the maximum of a shader stage
    

    




    Texture2D::Texture2D(uint32_t Unit  )  {
        ASSERT(Unit < GetMaxTextureUnits(), "Texture Unit greater than the maximum supported texture units !!!");
        //Generate default buffers and textures
        GLCALL(glGenTextures(1, &m_ObjectId));
        GLCALL(glBindTexture(GL_TEXTURE_2D, m_ObjectId));


        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        SetUnit(Unit);
        
    }

    

   

    Texture2D::~Texture2D(void){
        if (m_ObjectId != 0){
            GLCALL( glDeleteTextures(1 , &m_ObjectId) );
        }
        
    }

    
    uint32_t  Texture2D::GetUnit(void) const{
        return m_TextureUnit - GL_TEXTURE0;
    }

    void Texture2D::Bind(void) const {
        
        GLCALL( glActiveTexture(m_TextureUnit) );
        GLCALL( glBindTexture(GL_TEXTURE_2D , m_ObjectId) );
    }

    void Texture2D::Unbind(void) const {
        GLCALL( glActiveTexture(m_TextureUnit) );
        GLCALL( glBindTexture(GL_TEXTURE_2D , 0)); 
    }

    
    void Texture2D::SetUnit(uint32_t TextureUnit) { 
        if (TextureUnit >= GetMaxTextureUnits()){
            ASSERT(0 , "No more texture units can supplied :( !!!");
            m_TextureUnit = GL_TEXTURE0 + GetMaxTextureUnits() - 1;
            return ;  
        }
        m_TextureUnit = GL_TEXTURE0 + TextureUnit; 
    }

    
    void Texture2D::SetTexture(const Texture2DResolution res, void* data, TextureExternalFormat eformat) {
        Set(res, data, eformat);
    }

    void Texture2D::Set(
        Texture2DResolution res,
        void* data, 
        TextureExternalFormat external) {
        // clear all previus except unit
        Clear();
        // if there is no texture object then make one
        Bind();

        GLenum internalForm = GetOpenGLInternalTextureFormat(res.GetFormat());
        GLenum externalForm = GetOpenGLExternalTextureFormat(external);
        GLenum externalType = GetOpenGLExternalTextureType(external);

        GLCALL( glTexImage2D(
            GL_TEXTURE_2D,
            0,              // level (0 = base level)
            internalForm,        // internal format
            res.GetWidth(),
            res.GetHeight(),
            0,              // border (0)
            externalForm,        // format of your data
            externalType,
            data            // pointer στα δεδομένα
        ) );

        // Generate
        GLCALL( glGenerateMipmap(GL_TEXTURE_2D) );
    }
    void Texture2D::Clear(void) {
        // no need yet
        if (m_ObjectId != 0) {
            glDeleteTextures(1, &m_ObjectId);
            m_ObjectId = 0;
        }

        glGenTextures(1, &m_ObjectId);
        glBindTexture(GL_TEXTURE_2D, m_ObjectId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

	
    uint32_t  Texture2D::GetId(void) const { return m_ObjectId; }

}