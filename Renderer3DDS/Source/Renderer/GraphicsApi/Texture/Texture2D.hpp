#pragma once
#include <Defs.hpp>
#include "../Format/Format.hpp"
#include "Texture2DResolution.hpp"

namespace MultiStation{
    
    uint32_t* GetDefaultTexture2DData(uint32_t* width , uint32_t* height);

    static uint32_t GetMaxTextureUnits(void) ;
    
    /*Texture2D class represent a simple 2 Dimensions Immutable Texture 
      and bind that to a texture unit .*/
    class Texture2D{
        public:
            
            enum class Wrap2D{
                CLAMP_TO_EDGE   ,
                REPEAT          
            };
            

        public:
            Texture2D(uint32_t Unit = 0 ,const Texture2DResolution& MaxResolution = Texture2DResolution() );
            //Initiallize the storage for the texture in the device and set default data in it
            //if the initiallization failed to make the storage on the device then the method hasAllocated will return false
            
            ~Texture2D(void);
            

            bool HasAllocated(void) const;
            // Return true if the allocation for the texture (storage for the texture) was successfull or not

            bool AllocateStorage(void) ;
            //request storage if the previus request has failed , otherwise does nothing 
            //return false if allocating storage has failed again or if storage for the texture already exist's

            void SetUnit(uint32_t TextureUnit);

            uint32_t  GetUnit(void) const;

            void SetSubImage(void* data , uint32_t x , uint32_t y , uint32_t width , uint32_t height , TextureExternalFormat format );
            //sends the data to the device texture in the texels region (x,y) ==> (x + width , y)
            //                                                          (x,y) ==> (x , y+ height)
            void Bind(void) const;
    
            void Unbind(void) const;

            void SetFilter(Filter MagnificationFilter , Filter MinificationFilter);

            void SetWrap2D(const Wrap2D& wrap);

            uint32_t  GetId(void) const;

        private:
            uint32_t                 m_TextureUnit;          // texture unit that this texture will bound to
            uint32_t                 m_ObjectId;             // The Texture Object Name 
            uint32_t                 m_Sampler2DId;          // The Sampler object id 
            uint32_t                 m_PixelPackBufferId;    // A buffer that will used to change the data without binding or unbinding the texture and texture slot
            Texture2DResolution m_MaxResolution;        // The maximum resolution of the 2D Texture 
            bool                m_ProxyRequest;         // Tells if the allocation of the texture in vram is happen or not
        private:
            
            
    };



}