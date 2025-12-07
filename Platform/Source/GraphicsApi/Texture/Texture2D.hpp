#pragma once
#include "../../Definitions.h"
#include "../Format/Format.hpp"
#include "Texture2DResolution.hpp"

namespace MultiStation{
    
    uint32_t* GetDefaultTexture2DData(uint32_t* width , uint32_t* height);

    static uint32_t GetMaxTextureUnits(void) ;
    
    /*Texture2D class represent a simple 2 Dimensions Immutable Texture 
      and bind that to a texture unit .*/
    

    class Texture2D{
        

        public:
            Texture2D(uint32_t Unit = 0 );
            //Initiallize the storage for the texture in the device and set default data in it
            //if the initiallization failed to make the storage on the device then the method hasAllocated will return false
            
            ~Texture2D(void);
            
            void SetTexture( const Texture2DResolution res, void* data, TextureExternalFormat eformat);

            
            void SetUnit(uint32_t TextureUnit);

            uint32_t  GetUnit(void) const;

            
            void Bind(void) const;
    
            void Unbind(void) const;

            

            uint32_t  GetId(void) const;

        private:
            uint32_t                 m_TextureUnit;          // texture unit that this texture will bound to
            uint32_t                 m_ObjectId;             // The Texture Object Name 
            Texture2DResolution      m_Resolution;        // The maximum resolution of the 2D Texture 
            
        private:
            // re-initialize the texture data , internal and external formats as well the dimensions
            void Set(Texture2DResolution res, void* data , TextureExternalFormat external);
            void Clear(void);
            
    };



}