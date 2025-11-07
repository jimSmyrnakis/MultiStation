#pragma once

#include <Defs.hpp>
#include "../Format/Format.hpp"

namespace Game{
    class Texture2DResolution{
        public:
            Texture2DResolution(const u32& width = 1024 , const u32& height = 1024 , const TextureInternalFormat& format = TextureInternalFormat::RGBA8);

            void SetWidth (const u32&                   width );
            void SetHeight(const u32&                   height);
            void SetFormat(const TextureInternalFormat& format);

            u32                   GetWidth (void) const;
            u32                   GetHeight(void) const;
            TextureInternalFormat GetFormat(void) const;


        public:
            static u32 GetMaxWidth(void);
            static u32 GetMaxHeight(void);

        private:
            u32                         m_Width     ;
            u32                         m_Height    ;
            TextureInternalFormat       m_Format    ;
            bool                        m_BadW      ;
            bool                        m_BadH      ;              
    };
}