#include "FBuffer.hpp"
#include <GL/glew.h>
#include "../Init.hpp"
namespace MultiStation{

    FBuffer::FBuffer(
        const Texture2DResolution& maxResolution , 
        const DepthFormat& depthFormat  , 
        const uint32_t& ActiveColorBuffers,
        const Type type 
        
    ){
        //initiallize members
        m_Type = type;
        m_ColorBuffers = nullptr;
        m_ActiveColorBuffers = ActiveColorBuffers;
        m_BadState = false;
        m_DepthFormat = depthFormat;
        m_MaxResolution = maxResolution;
        m_ObjectId = 0;
        m_DepthId = 0;
        m_ViewPort = glm::vec4( 0 , 0, maxResolution.GetWidth() , maxResolution.GetHeight());

        //check parameters
        if (ActiveColorBuffers == 0){
            
            ASSERT(0 , "Error -- Frame buffer with no color buffers ? Active Color Buffers are set to 1 !!!!");
            m_ActiveColorBuffers = 1;
        }

        if (ActiveColorBuffers > 16){
            ASSERT(0 , "Error -- Max Color Buffer Attachments Supported are 8 !!! Set To max ");
            m_ActiveColorBuffers = 8;
        }


        

        InitBuffers();

    }

    void FBuffer::InitBuffers(void) {

        if (m_Type == Type::Screen) {
            // if is on screen then do nothing
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(m_ViewPort.x , m_ViewPort.y  , m_ViewPort.z , m_ViewPort.w);
            return;
        }
        
        //create the textures
        m_ColorBuffers = new Texture2D * [m_ActiveColorBuffers];

        for (int i = 0; i < m_ActiveColorBuffers; i++) {
            m_ColorBuffers[i] = new Texture2D(0); // Wish to have memory :)
            if (m_ColorBuffers[i] == nullptr) {
                ASSERT(0, "Failed to allocate memory for Texture 2D for Frame Buffer Color Attachment !!!!");
            }
            uint32_t trash;
            m_ColorBuffers[i]->SetTexture(m_MaxResolution, nullptr, TextureExternalFormat::RGB8);
        }

        // create names
        GLCALL( glGenFramebuffers(1 , &m_ObjectId) );
        GLCALL( glGenRenderbuffers(1 , &m_DepthId) );


        //bind the frame buffer object and the render buffer object 
        GLCALL( glBindFramebuffer(GL_FRAMEBUFFER, m_ObjectId) );
        GLCALL( glBindRenderbuffer(GL_RENDERBUFFER , m_DepthId) );

        //Set Depth Buffer
        GLCALL( glRenderbufferStorage(
            GL_RENDERBUFFER , 
            GetOpenGLDepthFormat(m_DepthFormat) , 
            m_MaxResolution.GetWidth() , 
            m_MaxResolution.GetHeight() ) );

        //Set this Depth Buffer to the Frame Buffer 
        GLCALL( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER , m_DepthId ) );

        // now set textures to the frame buffer object as the color attachments
        for (int i = 0; i < m_ActiveColorBuffers; i++){
            m_ColorBuffers[i]->Bind();
            uint32_t texId = m_ColorBuffers[i]->GetId();
            GLCALL( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i , GL_TEXTURE_2D , texId ,0 ) );
            
            
        }

        std::vector<GLenum> attachments;
        for (int i = 0; i < m_ActiveColorBuffers; ++i)
            attachments.push_back(GL_COLOR_ATTACHMENT0 + i);

        GLCALL( glDrawBuffers(attachments.size(), attachments.data()) );
        
        //Check if all have been Created as espected
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        m_BadState = true;
        switch(status){
            case GL_FRAMEBUFFER_COMPLETE: m_BadState = false; break;
            case GL_FRAMEBUFFER_UNDEFINED: ASSERT(0 , "No Default Frame Buffer has Given from the Window System !!!"); break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: 
            ASSERT(0 , "Attachment's of the Frame Buffer are not Set right !!!");
            break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            ASSERT(0 , "No image from Any Attachment of the Frame Buffer has Set right !!!");
            break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
            ASSERT(0 , "There is a no Supported Attachment for this Vendor !!!");
            break;
            default:
                ASSERT(0 , "Frame Buffer Failed to set , for unknown cause !!!");
        }

    }

    FBuffer::~FBuffer(void){
        if (m_Type == Type::Screen) {
            return;
        }
        if (m_ColorBuffers){
            for (int i = 0; i < m_ActiveColorBuffers; i++){
                delete m_ColorBuffers[i];
            }
            delete[] (m_ColorBuffers);
        }

        if (m_ObjectId) {
            GLCALL( glDeleteFramebuffers(1, &m_ObjectId) );
        }
        if (m_DepthId) {
            GLCALL(glDeleteRenderbuffers(1, &m_DepthId));
        }
        

    }

    
    uint32_t FBuffer::GetId(void) const { return m_ObjectId; }

    uint32_t FBuffer::GetColorBuffersCount(void) const { return m_ActiveColorBuffers; }

    void FBuffer::Bind(void) const{
        if (m_BadState){
            ASSERT(0 , "FrameBuffer is at  bad state :( !!!!");
            return;
        }
        // Render using the texture
        GLCALL( glBindFramebuffer(GL_FRAMEBUFFER, m_Type == Type::Screen ? 0 : m_ObjectId) );
        GLCALL(glViewport(m_ViewPort.x, m_ViewPort.y, m_ViewPort.z, m_ViewPort.w));
    }

    void FBuffer::Unbind(void) const{
        ASSERT(!m_BadState , "FrameBuffer is at  bad state :( !!!!");
        GLCALL( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
    }

    void FBuffer::ClearColorBuffer(const uint32_t& ActiveColorBuffer , const glm::vec4& rgba) const{
        if (m_BadState){
            ASSERT(0 , "FrameBuffer is at  bad state :( !!!!");
            return;
        }
        //check parameter
        if (ActiveColorBuffer >= m_ActiveColorBuffers){
            ASSERT(0 , "Out Of range !!!");
            return;
        }
        


        //bind this frame buffer and clear the specific active color buffer 
        GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_Type == Type::Screen ? 0 : m_ObjectId));
        if (m_Type != Type::Screen) {
            GLCALL(glDrawBuffer(GL_COLOR_ATTACHMENT0 + ActiveColorBuffer));
        }
        GLCALL(glClearColor(rgba.r, rgba.g, rgba.b, rgba.a));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT));
    }

    void FBuffer::ClearDepthBuffer(const float& clampValue) const{
        if (m_BadState){
            ASSERT(0 , "FrameBuffer is at  bad state :( !!!!");
            return;
        }
        

        //bind this frame buffer and clear the specific active color buffer 
        GLCALL( glBindFramebuffer(GL_FRAMEBUFFER, m_Type == Type::Screen ? 0 : m_ObjectId) );
        
        GLCALL( glClearDepth(clampValue) );
        GLCALL( glClear(GL_DEPTH_BUFFER_BIT) );
    }

    void FBuffer::SetResolution(const uint32_t& width , const uint32_t& height){
        if (m_BadState){
            ASSERT(0 , "FrameBuffer is at  bad state :( !!!!");
            return;
        }
        if ((width > m_MaxResolution.GetWidth()) || (height > m_MaxResolution.GetHeight())){
            ASSERT(0 , "Out Of range !!!");
            return;
        }
        
        m_ViewPort = glm::vec4(0 , 0, width , height);
    }

    Texture2D* FBuffer::GetColorBuffer(const uint32_t& ActiveColorBuffer) const{
        if (m_BadState){
            ASSERT(0 , "FrameBuffer is at  bad state :( !!!!");
            return nullptr;
        }
        //check parameter
        if (ActiveColorBuffer >= m_ActiveColorBuffers){
            ASSERT(0 , "Out Of range !!!");
            return nullptr;
        }

        return m_ColorBuffers[ActiveColorBuffer];
    }

}