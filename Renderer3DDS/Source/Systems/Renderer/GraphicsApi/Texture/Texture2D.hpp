#pragma once
#include <Defs.hpp>
#include <glad/gl.h>
#include "../Format/Format.hpp"
#include "Texture2DResolution.hpp"

namespace Game{
    
    u32* GetDefaultTexture2DData(u32* width , u32* height);

    static u32 GetMaxTextureUnits(void) ;
    
    /*Texture2D class represent a simple 2 Dimensions Immutable Texture 
      and bind that to a texture unit .*/
    class Texture2D{
        public:
            
            enum class Wrap2D{
                CLAMP_TO_EDGE   ,
                REPEAT          
            };
            

        public:
            Texture2D(u32 Unit = 0 ,const Texture2DResolution& MaxResolution = Texture2DResolution() );
            //Initiallize the storage for the texture in the device and set default data in it
            //if the initiallization failed to make the storage on the device then the method hasAllocated will return false
            
            ~Texture2D(void);
            

            bool HasAllocated(void) const;
            // Return true if the allocation for the texture (storage for the texture) was successfull or not

            bool AllocateStorage(void) ;
            //request storage if the previus request has failed , otherwise does nothing 
            //return false if allocating storage has failed again or if storage for the texture already exist's

            void SetUnit(u32 TextureUnit);

            u32  GetUnit(void) const;

            void SetSubImage(void* data , u32 x , u32 y , u32 width , u32 height , TextureExternalFormat format );
            //sends the data to the device texture in the texels region (x,y) ==> (x + width , y)
            //                                                          (x,y) ==> (x , y+ height)
            void Bind(void) const;
    
            void Unbind(void) const;

            void SetFilter(Filter MagnificationFilter , Filter MinificationFilter);

            void SetWrap2D(const Wrap2D& wrap);

            u32  GetId(void) const;

        private:
            u32                 m_TextureUnit;          // texture unit that this texture will bound to
            u32                 m_ObjectId;             // The Texture Object Name 
            u32                 m_Sampler2DId;          // The Sampler object id 
            u32                 m_PixelPackBufferId;    // A buffer that will used to change the data without binding or unbinding the texture and texture slot
            Texture2DResolution m_MaxResolution;        // The maximum resolution of the 2D Texture 
            bool                m_ProxyRequest;         // Tells if the allocation of the texture in vram is happen or not
        private:
            
            
    };



}