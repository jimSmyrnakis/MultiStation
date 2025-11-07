#pragma once
#include <Defs.hpp>
#include "../Format/Format.hpp"
namespace Game{
    
    



    struct Color32F {
        float r;
        float g;
        float b;
        float a;
    };

    class FBuffer{
        
        public: 
            FBuffer(void);
            ~FBuffer(void) ;

            
            FBuffer(const FBuffer& copy) = delete;
            FBuffer operator=(const FBuffer& copy) = delete;
            FBuffer(FBuffer&& move) = delete;
            FBuffer& operator=(FBuffer&& move) = delete;
        
        public:
            void SetSize(u32 Width , u32 Height);
            void SetDepthBuffer( float ClearValue , DepthFormat format );
            void SetColorBuffer(const Color32F& ClearColor , TextureInternalFormat format);

            u32         GetWidth(void)          const;
            u32         GetHeight(void)         const;
            float       GetClearDepth(void)     const;
            DepthFormat GetDepthFormat(void)    const;
            Color32F    GetClearColor(void)     const;
            TextureInternalFormat GetColorForamt(void)    const;
            /*Simple function's that return the format's that last set , that is not mean that they have applied */

        

        public:
            void Create(void) const;
            /*This method is responsible for creating the new render buffer's if they do not exist's or change their 
              Size and format to the desire one if they already exist's , as attach them to the frame buffer */
            void Clear(void)  const;
            /*This method is responsible for clearing every buffer that has set enable (except the color buffer) to their 
              clear value that has set , if the binded one frame buffer is not this then still is going to clear the frame
              buffer attachments (The Active ones) and the binded frame buffer will remain as was .  
              If there is a case where this buffer have never call the Create method for the initiallize of the Frame Buffer 
              then an error breakpoint will be generated */
            void Bind(void) const; /*
             enables and set the frame buffer and all the other settings 
             , if no call to Create method has happen a debug exception is generate in the form 
             of breakpoint (as im not much of using throw exception , these will make code harder to handle ) */
            void Unbind(void) const; 
            /*  disables the frame buffer and set the default one . Because setting's from the 
                default one are not know for moment will consider that is only going to used for his 
                color buffer 
            */
            void Copy(u32 x , u32 y , u32 width , u32 height ) const;
            
            //void Copy(FBuffer& buffer ,  Filter filter , bool DepthBuffer );
            /*  As Copy but insted of coping from this buffer data to the default system buffer , you can do that beetwen other 
                offsceen frame buffer , plus u can copy the Depth buffer too */ 
        private: /* Variables relative to the Frame Buffer */

            i32             m_BufferId; // The frame buffer id , by default is zero (the contex one).
            u32             m_Width;    // frame buffer width of Pixels
            u32             m_Height;   // Frame Buffer Height of Pixels
            

        private: /* Variables relative to the Color Buffer of the Frame Buffer */
            
            i32             m_ColorBufferId; 
            // Frame Buffer , the Color buffer id of it . Is set by default to zero that means no 
            // color buffer is generated 
            Color32F        m_BackGround; // RGBA default color buffer clear color
            TextureInternalFormat     m_CFormat; // Image Format for the Color Buffer 
        
        private: /* Variables relative to the Depth Buffer of the Frame Buffer */

            float           m_ClearDepth; // Depth Default Value when clearing the depth buffer 
            i32             m_DepthBufferId; 
            // Frame Buffer , the Depth Buffer id of it . Is set by default to zero that means 
            // no depth buffer is generated yet for this frame buffer
            DepthFormat     m_DFormat; // Depth Format for the Depth Buffer (Z-Buffer)
            
        private:
            
        }; // We going to support one color buffer for the moment as we target to render in a off screen and the provide
           // a copy method that will copy the image back to the back color buffer (left or right for dual camera systems ) 

}