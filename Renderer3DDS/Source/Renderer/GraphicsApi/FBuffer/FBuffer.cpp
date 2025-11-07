#include "FBuffer.hpp"

namespace Game{

    FBuffer::FBuffer(
        const Texture2DResolution& maxResolution , 
        const DepthFormat& depthFormat  , 
        const u32& ActiveColorBuffers ,
        alloc       AllocateFunc  ,
        dalloc      FreeFunc 
    ){
        //initiallize members
        m_ColorBuffers = nullptr;
        m_ActiveColorBuffers = ActiveColorBuffers;
        m_BadState = false;
        m_DepthFormat = depthFormat;
        m_Malloc = AllocateFunc;
        m_Free = FreeFunc;
        m_MaxResolution = maxResolution;
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
        if (AllocateFunc == nullptr){
            ASSERT(0 , "Warning -- null pointer for allocation function - default malloc is selected !!!");
            m_Malloc = malloc;
        }
        if (FreeFunc == nullptr){
            ASSERT(0 , "Warning -- null pointer for free memory function - default free is selected !!!");
            m_Free = free;
        }

        //create the textures
        m_ColorBuffers = (Texture2D**)m_Malloc(sizeof(Texture2D*));
        for (int i =0 ; i < m_ActiveColorBuffers; i++){
            m_ColorBuffers[i] = new Texture2D(0 , m_MaxResolution); // Wish to have memory :)
        }

        InitBuffers();

    }

    void FBuffer::InitBuffers(void) {

        
        // create names
        GLCALL( glGenFramebuffers(1 , &m_ObjectId) );
        GLCALL( glGenRenderbuffers(1 , &m_DepthId) );

        // Take old one frame buffer (Basicly after this function end the previus frame buffer must be already bind)
        i32 OldFB ;
        GLCALL( glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &OldFB) );

        //bind the frame buffer object and the render buffer object 
        GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER , m_ObjectId) );
        GLCALL( glBindRenderbuffer(GL_RENDERBUFFER , m_DepthId) );

        //Set Depth Buffer
        GLCALL( glRenderbufferStorage(
            GL_RENDERBUFFER , 
            GetOpenGLDepthFormat(m_DepthFormat) , 
            m_MaxResolution.GetWidth() , 
            m_MaxResolution.GetHeight() ) );
        //Set this Depth Buffer to the Frame Buffer 
        GLCALL( glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER , GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER , m_DepthId ) );

        // now set textures to the frame buffer object as the color attachments
        for (int i = 0; i < m_ActiveColorBuffers; i++){
            m_ColorBuffers[i]->Bind();
            u32 texId = m_ColorBuffers[i]->GetId();
            GLCALL( glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 + i , GL_TEXTURE_2D , texId ,0 ) );
            
        }
        
        //Check if all have been Created as espected
        GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
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

        //Bind the old one frame buffer 
        GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER , OldFB) );
    }

    FBuffer::~FBuffer(void){
        if (m_ColorBuffers){
            for (int i = 0; i < m_ActiveColorBuffers; i++){
                delete m_ColorBuffers[i];
            }
            m_Free(m_ColorBuffers);
        }

    }

    
    u32 FBuffer::GetId(void) const { return m_ObjectId; }

    u32 FBuffer::GetColorBuffersCount(void) const { return m_ActiveColorBuffers; }

    void FBuffer::Bind(void) const{
        if (m_BadState){
            ASSERT(0 , "FrameBuffer is at  bad state :( !!!!");
            return;
        }
        glViewport(m_ViewPort.x , m_ViewPort.y , m_ViewPort.z , m_ViewPort.w );
        // Render using the texture
        glEnable(GL_TEXTURE_2D);
        m_ColorBuffers[0]->Bind();
        GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER , m_ObjectId) );
    }
    void FBuffer::Unbind(void) const{
        ASSERT(!m_BadState , "FrameBuffer is at  bad state :( !!!!");
        GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER , 0) );
    }

    void FBuffer::ClearColorBuffer(const u32& ActiveColorBuffer , const glm::vec4& rgba) const{
        if (m_BadState){
            ASSERT(0 , "FrameBuffer is at  bad state :( !!!!");
            return;
        }
        //check parameter
        if (ActiveColorBuffer >= m_ActiveColorBuffers){
            ASSERT(0 , "Out Of range !!!");
            return;
        }
        // Take old one frame buffer (Basicly after this function end the previus frame buffer must be already bind)
        i32 OldFB ;
        GLCALL( glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &OldFB) );

        //bind this frame buffer and clear the specific active color buffer 
        GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER , m_ObjectId) );
        GLCALL( glClearColor(rgba.r , rgba.g , rgba.b , rgba.a ) );
        GLCALL( glClear(GL_COLOR_BUFFER_BIT) );

        //Bind the old one frame buffer 
        GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER , OldFB) );
    }
    void FBuffer::ClearDepthBuffer(const float& clampValue) const{
        if (m_BadState){
            ASSERT(0 , "FrameBuffer is at  bad state :( !!!!");
            return;
        }
        // Take old one frame buffer (Basicly after this function end the previus frame buffer must be already bind)
        i32 OldFB ;
        GLCALL( glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &OldFB) );

        //bind this frame buffer and clear the specific active color buffer 
        GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER , m_ObjectId) );
        GLCALL( glClearDepthf(clampValue) );
        GLCALL( glClear(GL_DEPTH_BUFFER_BIT) );
        //Bind the old one frame buffer 
        GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER , OldFB) );
    }

    void FBuffer::SetResolution(const u32& width , const u32& height){
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

    Texture2D* FBuffer::GetColorBuffer(const u32& ActiveColorBuffer) const{
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