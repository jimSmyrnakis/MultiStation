#include "FBuffer.hpp"
#include "../Format/Format.hpp"
#include <glad/gl.h>

namespace Game{
    
            /*Every special method like the destructor , and methods like Create , Copy , Clear must not change the state of 
              OpenGL Api Frame Buffer that it was currently bounded */

            /*Constructors and destructor */
            FBuffer::FBuffer(void){
                //create a frame buffer object name
                GLCALL( glGenFramebuffers(1 , (GLuint*)&m_BufferId) );
                // set default values for predefined behavor if nothing going to set
                // before Create methods called
                //frame buffer
                m_Width = 400;
                m_Height= 400;

                //color buffer 
                m_BackGround.r = m_BackGround.g = m_BackGround.b = 0.1f;
                m_BackGround.a = 1.0f;
                m_CFormat      = TextureInternalFormat::RGB8;
                m_ColorBufferId= 0; // set no color buffer yet

                //depth buffer
                m_ClearDepth   = 1.0f; // the farest point of view after transforming z unit cube coordinates to Window Coordinates
                m_DFormat      = DepthFormat::DEPTH24;
                m_DepthBufferId= 0; // set no depth buffer yet

            }
            /* Default set for the frame buffer for pre-defined behavor , plus a frame buffer generation */
            FBuffer::~FBuffer(void){
                i32 CurrentFBO;
                // Just for security reasons
                ASSERT(m_BufferId , "FBuffer Frame Buffer Object Name is zero !!!");
                // Get Current Frame Buffer State 
                GLCALL( glGetIntegerv(GL_FRAMEBUFFER_BINDING, &CurrentFBO) );
                // Bind the frame buffer 
                GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER , m_BufferId) );
                // now that the frame buffer is binded we must dallocate the image data of the color 
                // buffer first and then the object color buffer too 
                if (m_ColorBufferId) // if color buffer object name is created then delete
                {
                    //first bind the render buffer - color buffer
                    GLCALL( glBindRenderbuffer(GL_RENDERBUFFER , m_ColorBufferId) );
                    //de-attach color buffer from frame buffer
                    GLCALL( glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 , GL_RENDERBUFFER , 0) );
                    //delete render buffer (Color Buffer) and all data associate with it
                    GLCALL( glDeleteRenderbuffers(1 , (GLuint*)&m_ColorBufferId) );
                }
                // See if Depth Buffer has defined
                if (m_DepthBufferId){

                    //first bind render buffer - depth buffer
                    GLCALL( glBindRenderbuffer(GL_RENDERBUFFER , m_DepthBufferId) );
                    //delete all the image data (depth more accurate) for this depth buffer
                    GLCALL( glInvalidateFramebuffer(GL_DRAW_FRAMEBUFFER , 1 , (GLuint*)&m_DepthBufferId) );
                    //de-attach depth buffer from frame buffer
                    GLCALL( glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER , GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER , 0) );
                    //delete render buffer object (Depth buffer object)
                    GLCALL( glDeleteRenderbuffers(1 , (GLuint*)&m_DepthBufferId) );
                }
                

                // delete frame buffer object
                GLCALL( glDeleteFramebuffers(1 , (GLuint*)&m_BufferId) );
                // now restore for binding the last frame buffer
                GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER , CurrentFBO) );
            }
            /*All renderBuffer's (Color , Depth , Stencil ) must deleted and the Frame Buffer after , 
            as the current frame buffer must still stay as is in the OpenGL State*/

            
            /*Frame Buffer Set Up method's : Methods that are not creating render buffer's but they set how to create them 
                 or be able to see what is created */
            void FBuffer::SetSize(u32 Width , u32 Height){
                if ( (!Width) || (!Height) ){
                    ASSERT(Width , "Warning - Width is zero !!!");
                    ASSERT(Height, "Warning - Height is Zero !!!");
                    return;
                }
                m_Width = Width;
                m_Height= Height;
            }
            void FBuffer::SetDepthBuffer( float ClearValue , DepthFormat format ){
                m_ClearDepth = ClearValue;
                m_DFormat    = format;
            }
            void FBuffer::SetColorBuffer(const Color32F& ClearColor , TextureInternalFormat format){
                m_BackGround = ClearColor ;
                m_CFormat    = format     ;
            }
            /*These methods do not change the render buffer's size's of data or allocate them for first time 
              but they just set the desire size's of data for the render buffer's . It is the create method 
              that do the opengl calls to allocate the new render buffer's and is responsible for deleting 
              previus buffers and allocate new one's or create them for the first time */

            u32         FBuffer::GetWidth(void)          const { return m_Width;  }
            u32         FBuffer::GetHeight(void)         const { return m_Height; }
            float       FBuffer::GetClearDepth(void)     const { return m_ClearDepth; }
            DepthFormat FBuffer::GetDepthFormat(void)    const { return m_DFormat; }
            Color32F    FBuffer::GetClearColor(void)     const { return m_BackGround; }
            TextureInternalFormat FBuffer::GetColorForamt(void)    const { return m_CFormat; }
            /*Simple function's that return the format's that last set , that is not mean that they have applied */

            
            /**/
            void FBuffer::Create(void) const{
                // test for security reasons if the frame buffer exist
               // ASSERT(glIsFramebuffer(m_BufferId) == GL_TRUE , "Frame Buffer is not exist's !!!");

                i32 CurrentFBO;
                //Get Current Frame Buffer
                GLCALL( glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &CurrentFBO) );
                //Set This Frame Buffer for Fether Process
                GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER , m_BufferId) );

                //Check if Color Buffer not Exists Yet , if yes then Generate him
                if (m_ColorBufferId == 0){
                    GLCALL( glGenRenderbuffers(1 , (GLuint*)&m_ColorBufferId) );
                }
                else{ // if previus one exist then delete the Image Data

                    // Remove color buffer attachment0 from the frame buffer
                    GLCALL( glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 , GL_RENDERBUFFER , 0) );
                    // Delete the old renderbuffer , the image of this render buffer will removed from all attach
                    // points of the currently frame buffer but not from the non bound frame buffer's 
                    GLCALL( glDeleteRenderbuffers(1 , (GLuint*)&m_ColorBufferId) );
                    // recrate a new render buffer
                    GLCALL( glGenRenderbuffers(1 , (GLuint* )&m_ColorBufferId) );
                }
                //Bind the new Render Buffer
                GLCALL( glBindRenderbuffer(GL_RENDERBUFFER , m_ColorBufferId) );
                //check if width and height is zero
                ASSERT(m_Width , "Width of the Frame buffer is set to zero !!!");
                ASSERT(m_Height, "Height of the Frame Buffer is Zero !!!");
                //Check if width or Height is bigger than the maximum
                ASSERT(m_Width <= GL_MAX_RENDERBUFFER_SIZE , "Width is out of bounds !!!");
                ASSERT(m_Height <= GL_MAX_RENDERBUFFER_SIZE , "Height is out of Bounds !!!");
                //Set the new Storage of this color buffer
                GLCALL( glRenderbufferStorage(GL_RENDERBUFFER , GetOpenGLInternalTextureFormat(m_CFormat) , m_Width , m_Height ) );
                //Attach the new Render Buffer to the Color buffer
                GLCALL( glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 , GL_RENDERBUFFER , m_ColorBufferId) );
                
                //Depth Buffer
                //Check if exist a Depth Buffer , if no create a object name
                if (m_DepthBufferId == 0){
                    GLCALL( glGenRenderbuffers(1 , (GLuint*)&m_DepthBufferId) );
                }
                else{ // if previus one exist then delete the Image 

                    // Remove depth buffer attachment from the frame buffer
                    GLCALL( glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER , GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER , 0) );
                    // Delete the old renderbuffer , the image of this render buffer will removed from all attach
                    // points of the currently frame buffer but not from the non bound frame buffer's 
                    GLCALL( glDeleteRenderbuffers(1 , (GLuint*)&m_DepthBufferId) );
                    // recrate a new render buffer
                    GLCALL( glGenRenderbuffers(1 , (GLuint* )&m_DepthBufferId) );
                }
                //Bind the new Depth Buffer
                GLCALL( glBindRenderbuffer(GL_RENDERBUFFER , m_DepthBufferId) );
                //Set the new image of this depth buffer
                GLCALL( glRenderbufferStorage(GL_RENDERBUFFER, GetOpenGLDepthFormat(m_DFormat) , m_Width , m_Height ) );
                //Attach the new Render Buffer to the Depth Attachment
                GLCALL( glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER , GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER , m_DepthBufferId) );

                //Check if all have been Created as espected
                GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
                switch(status){
                    case GL_FRAMEBUFFER_COMPLETE: break;
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

                //now bind the current one frame buffer that was binded before this method called
                GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER , (GLuint)CurrentFBO) );
            }
            /*This method is responsible for creating the new render buffer's if they do not exist's or change their 
              Size and format to the desire one if they already exist's , as attach them to the frame buffer */
            void FBuffer::Clear(void)  const{
                // test for security reasons if the frame buffer exist
                ASSERT(glIsFramebuffer(m_BufferId) == GL_TRUE , "Frame Buffer is not exist's !!!");

                i32 CurrentFBO;
                //Get Current Frame Buffer
                GLCALL( glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &CurrentFBO) );
                //Set This Frame Buffer for Fether Process
                GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER , m_BufferId) );

                GLbitfield ClearBuffersFlags = GL_COLOR_BUFFER_BIT;
                //check if color buffer exist's
                ASSERT(m_ColorBufferId , "Frame Buffer has not initiallized yet");

                //Check if Depth Buffer is enable
                if (m_DepthBufferId)
                    ClearBuffersFlags |= GL_DEPTH_BUFFER_BIT;
                //Set Clear Color for clearing the Color Buffer Image to
                GLCALL( glClearColor(m_BackGround.r , m_BackGround.g , m_BackGround.b , m_BackGround.a) );
                //Set Clear Depth for clearing the Depth Buffer Image to
                GLCALL( glClearDepth(m_ClearDepth) );
                
                //Clear The Buffers
                GLCALL( glClear(ClearBuffersFlags) );

                //restore the Binded one Frame Buffer
                GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER , CurrentFBO) );
            }
            /*This method is responsible for clearing every buffer that has set enable (except the color buffer) to their 
              clear value that has set , if the binded one frame buffer is not this then still is going to clear the frame
              buffer attachments (The Active ones) and the binded frame buffer will remain as was .  
              If there is a case where this buffer have never call the Create method for the initiallize of the Frame Buffer 
              then an error breakpoint will be generated */
            void FBuffer::Bind(void) const{
                //check if no call to the Create method has happen
                ASSERT( m_ColorBufferId , "Frame Buffer is posible that has not initiallized yet , call Create First !!!");
                //Set ViewPort to the set analysis 
                //GLCALL( glViewport(0 , 0 , m_Width , m_Height) );
                //just bind the frame buffer
                GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_BufferId) );
                
            } /*
             enables and set the frame buffer and all the other settings 
             , if no call to Create method has happen a debug exception is generate in the form 
             of breakpoint (as im not much of using throw exception , these will make code harder to handle ) */
            void FBuffer::Unbind(void) const{
                //check if no call to the Create method has happen
                ASSERT( m_ColorBufferId , "Frame Buffer is posible that has not initiallized yet , call Create First !!!");
                //just bind the default frame buffer
                GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0) );
                
            }
            /*  disables the frame buffer and set the default one . Because setting's from the 
                default one are not know for moment will consider that is only going to used for his 
                color buffer 
            */
            void FBuffer::Copy(u32 x , u32 y , u32 width , u32 height ) const{
                // test for security reasons if the frame buffer exist
                ASSERT(glIsFramebuffer(m_BufferId) == GL_TRUE , "Frame Buffer is not exist's !!!");

                i32 Current_Draw_FBO;
                i32 Current_Read_FBO;
                //Get Current Draw Frame Buffer
                GLCALL( glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &Current_Draw_FBO) );
                //Get Current Read Frame Buffer
                GLCALL( glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &Current_Read_FBO ) );
                //Set This Frame Buffer for Reading Process
                GLCALL( glBindFramebuffer(GL_READ_FRAMEBUFFER , m_BufferId) );
                //Set The Default Frame Buffer for Drawing
                GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER , 0) );
                //Clear the Color Buffer
                //GLCALL( glClearColor(m_BackGround.r , m_BackGround.g , m_BackGround.b , m_BackGround.a) );

                
                //Copy from this Color Buffer to the back buffer of the Window Frame Buffer
                GLCALL( glBlitFramebuffer(
                    0 , 0 , m_Width    -1 ,  m_Height-1 , 
                    x , y , width      -1 , height      -1 , 
                    GL_COLOR_BUFFER_BIT   , GL_LINEAR  
                ) );

                //Set The Old one's
                GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER ,  Current_Draw_FBO) );
                GLCALL( glBindFramebuffer(GL_READ_FRAMEBUFFER ,  Current_Read_FBO) );
            }
            
}