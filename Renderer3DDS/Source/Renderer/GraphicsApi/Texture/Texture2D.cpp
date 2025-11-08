#include "Texture2D.hpp"

#include <GL/glew.h>
namespace MultiStation {
    #define DEFAULT_TEX2D_DIM 2048

    uint32_t DefaultTexture2D[DEFAULT_TEX2D_DIM][DEFAULT_TEX2D_DIM] = {0xFF0000FF};

    //TODO - Request the maximum number of texture units that can be active at a draw
    uint32_t GetMaxTextureUnits(void) { return 16; } // for moment 16 is sure the maximum of a shader stage
    
    Texture2D::Texture2D(uint32_t Unit  ,const Texture2DResolution& MaxResolution)  {
		ASSERT(Unit < GetMaxTextureUnits(), "Texture Unit greater than the maximum supported texture units !!!");


        SetUnit(Unit);
        m_MaxResolution = MaxResolution;
        m_ProxyRequest = false;

        //Generate default buffers and textures
        GLCALL( glGenTextures(1 , &m_ObjectId) );
        GLCALL( glGenSamplers(1 , &m_Sampler2DId) );
        GLCALL( glGenBuffers( 1 , &m_PixelPackBufferId ) );

        //Enable Texture Unit
        GLCALL( glActiveTexture(m_TextureUnit) );
        
        //Bind all buffers and textures 
        GLCALL( glBindTexture(GL_TEXTURE_2D , m_ObjectId) );
        GLCALL( glBindBuffer( GL_PIXEL_UNPACK_BUFFER , m_PixelPackBufferId) );
        GLCALL( glBindSampler(m_TextureUnit - GL_TEXTURE0 , m_Sampler2DId ) );

        //default values for the sampler => filters and behavors
        GLCALL( glSamplerParameteri(m_Sampler2DId , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE ) );
        GLCALL( glSamplerParameteri(m_Sampler2DId , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE ) );
        GLCALL( glSamplerParameteri(m_Sampler2DId , GL_TEXTURE_MIN_FILTER , GL_LINEAR));
        GLCALL( glSamplerParameteri(m_Sampler2DId , GL_TEXTURE_MAG_FILTER , GL_LINEAR));

        //default values for the pixel unpack buffer 
        uint32_t trash;
        GLCALL( glBufferData( GL_PIXEL_UNPACK_BUFFER , sizeof(DefaultTexture2D) , GetDefaultTexture2DData(&trash , &trash), GL_STATIC_DRAW));


        AllocateStorage();

        
        
    }

    bool Texture2D::AllocateStorage(void) {
        if (m_ProxyRequest == true) // the storage is already allocated on the device memory
            return false;

        //Enable Texture Unit
        GLCALL( glActiveTexture(m_TextureUnit) );
        //Bind texture 
        GLCALL( glBindTexture(GL_TEXTURE_2D , m_ObjectId) );
        /*A proxy request for allocating internal storage for the texture - a check if that will succeded */
        GLCALL( glTexStorage2D(
            GL_PROXY_TEXTURE_2D                                     ,    
            1                                                       , 
            GetOpenGLInternalTextureFormat(m_MaxResolution.GetFormat())  ,
            m_MaxResolution.GetWidth() , m_MaxResolution.GetHeight()          ) );
        int32_t TextureWidth , TextureHeight;
        GLCALL( glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &TextureWidth) );
        GLCALL( glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &TextureHeight));
        if ((TextureHeight == 0) || (TextureWidth == 0)){
            m_ProxyRequest = false;
            ASSERT(0 , "No Available space for the texture !!!");
            return false;
        }

        // if the proxy requet is succeded then allocate that storage in real
        GLCALL( glTexStorage2D(
            GL_TEXTURE_2D , 1 , 
            GetOpenGLInternalTextureFormat(m_MaxResolution.GetFormat()) , 
            m_MaxResolution.GetWidth() , m_MaxResolution.GetHeight()) );

        // bind the pixel buffer 
        GLCALL( glBindBuffer( GL_PIXEL_UNPACK_BUFFER , m_PixelPackBufferId) );
        // set the default data 
        GLCALL( glBufferData(GL_PIXEL_UNPACK_BUFFER , sizeof(DefaultTexture2D) , DefaultTexture2D , GL_STATIC_DRAW ) );
        //Load a default data for the texture 
        uint32_t width , height ;
        width = height = DEFAULT_TEX2D_DIM ;
        if (m_MaxResolution.GetWidth() < DEFAULT_TEX2D_DIM)
            width = m_MaxResolution.GetWidth();
        if (m_MaxResolution.GetHeight() < DEFAULT_TEX2D_DIM)
            height= m_MaxResolution.GetHeight();
        GLCALL( glTexSubImage2D(GL_TEXTURE_2D , 0 , 0 , 0 , width , height , GL_RGBA , GL_UNSIGNED_BYTE , nullptr ) );

        m_ProxyRequest = true;
        return true;
    }

    Texture2D::~Texture2D(void){
        if (m_ObjectId != 0){
            GLCALL( glDeleteTextures(1 , &m_ObjectId) );
        }
        
        if (m_Sampler2DId != 0){
            GLCALL( glDeleteSamplers(1 , &m_Sampler2DId) );
        }
        
        if (m_PixelPackBufferId != 0){
            GLCALL( glDeleteBuffers(1 , &m_PixelPackBufferId) );
        }
        
    }

    
    uint32_t  Texture2D::GetUnit(void) const{
        return m_TextureUnit - GL_TEXTURE0;
    }

    void Texture2D::Bind(void) const {
        if (!m_ProxyRequest){
            ASSERT(0 , "Texture2D has no space on vram !!!");
            Unbind();
            return;
        }
        GLCALL( glActiveTexture(m_TextureUnit) );
        GLCALL( glBindTexture(GL_TEXTURE_2D , m_ObjectId) );
        GLCALL( glBindSampler(m_TextureUnit - GL_TEXTURE0 , m_Sampler2DId) );
        GLCALL( glBindBuffer(GL_PIXEL_UNPACK_BUFFER , m_PixelPackBufferId) );
    }

    void Texture2D::Unbind(void) const {
        GLCALL( glActiveTexture(m_TextureUnit) );
        GLCALL( glBindTexture(GL_TEXTURE_2D , 0)); 
        GLCALL( glBindSampler(m_TextureUnit - GL_TEXTURE0 , 0) );
        GLCALL( glBindBuffer(GL_PIXEL_UNPACK_BUFFER , 0) );
    }

    void Texture2D::SetSubImage(void* data , uint32_t x , uint32_t y , uint32_t width , uint32_t height , TextureExternalFormat format){
        if (!m_ProxyRequest){
            ASSERT(0 , "Texture2D has no space on vram !!!");
            return;
        }
        //check if the dimensions of the given data fit in the texture
        if (((x + width)  > m_MaxResolution.GetWidth())   || 
            ((y + height) > m_MaxResolution.GetHeight())  ||
            ( width == 0 ) || (height == 0)){
                ASSERT(0 , "OUT of Range !!!");
                return;
        }

        GLCALL(glActiveTexture(m_TextureUnit));
        GLCALL(glBindTexture(GL_TEXTURE_2D , m_ObjectId));
        uint32_t ExternalFormat , DataType ;
        ExternalFormat = GetOpenGLExternalTextureFormat(format);
        DataType = GetOpenGLExternalTextureType(format);
        
        GLCALL( glBindBuffer(GL_PIXEL_UNPACK_BUFFER , m_PixelPackBufferId) );
        GLCALL( glBufferData(GL_PIXEL_UNPACK_BUFFER  , width*height* 4 , data , GL_STATIC_DRAW ) );
        GLCALL(glTexSubImage2D(GL_TEXTURE_2D , 0 , x , y , width , height , ExternalFormat , DataType , nullptr));

    }

    bool Texture2D::HasAllocated(void) const { return m_ProxyRequest; }

    void Texture2D::SetUnit(uint32_t TextureUnit) { 
        if (TextureUnit >= GetMaxTextureUnits()){
            ASSERT(0 , "No more texture units can supplied :( !!!");
            m_TextureUnit = GL_TEXTURE0 + GetMaxTextureUnits() - 1;
            return ;  
        }
        m_TextureUnit = GL_TEXTURE0 + TextureUnit; 
    }

    void Texture2D::SetFilter(Filter MagnificationFilter , Filter MinificationFilter){
        ASSERT( 
            (MagnificationFilter==Filter::LINEAR) || (MagnificationFilter==Filter::NEAREST) , 
            "Magnification Filter is not a valid 2D Texture Filter !");

        ASSERT( 
            (MinificationFilter==Filter::LINEAR) || (MinificationFilter==Filter::NEAREST) , 
            "Minification Filter is not a valid 2D Texture Filter !");

        
        GLCALL( glSamplerParameteri(m_Sampler2DId , GL_TEXTURE_MAG_FILTER , GetOpenGLFilter(MagnificationFilter)));
        GLCALL( glSamplerParameteri(m_Sampler2DId , GL_TEXTURE_MIN_FILTER , GetOpenGLFilter(MinificationFilter)));

    }

    void Texture2D::SetWrap2D(const Texture2D::Wrap2D& wrap) {
        GLint gl_wrap;
        switch( wrap){
            case Wrap2D::CLAMP_TO_EDGE      :   gl_wrap = GL_CLAMP_TO_EDGE  ;   break;
            case Wrap2D::REPEAT             :   gl_wrap = GL_REPEAT         ;   break;
            default                         : 
            ASSERT(0 , "Undefined Wrap2D enum value !!!"); return;
        }
        
        GLCALL( glSamplerParameteri(m_Sampler2DId , GL_TEXTURE_WRAP_S , gl_wrap) );
        GLCALL( glSamplerParameteri(m_Sampler2DId , GL_TEXTURE_WRAP_T , gl_wrap) );
    }

	static bool isInitiallized = false;
    uint32_t* GetDefaultTexture2DData(uint32_t* width , uint32_t* height){
        (*width) = (*height) = DEFAULT_TEX2D_DIM;
		if (isInitiallized == false)
            for (uint32_t y = 0; y < DEFAULT_TEX2D_DIM; ++y)
                for (uint32_t x = 0; x < DEFAULT_TEX2D_DIM; ++x)
                    DefaultTexture2D[y][x] = x * y * 1024;
		isInitiallized = true;
        return (uint32_t*)DefaultTexture2D;
    }

    uint32_t  Texture2D::GetId(void) const { return m_ObjectId; }

}