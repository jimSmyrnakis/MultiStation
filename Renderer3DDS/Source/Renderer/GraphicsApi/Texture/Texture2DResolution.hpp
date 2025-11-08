#pragma once

#include <Defs.hpp>
#include "../Format/Format.hpp"

namespace MultiStation{
    class Texture2DResolution{
        public:
            Texture2DResolution(const uint32_t& width = 1024 , const uint32_t& height = 1024 , const TextureInternalFormat& format = TextureInternalFormat::RGBA8);

            void SetWidth (const uint32_t&                   width );
            void SetHeight(const uint32_t&                   height);
            void SetFormat(const TextureInternalFormat& format);

            uint32_t                   GetWidth (void) const;
            uint32_t                   GetHeight(void) const;
            TextureInternalFormat GetFormat(void) const;


        public:
            static uint32_t GetMaxWidth(void);
            static uint32_t GetMaxHeight(void);

        private:
            uint32_t                    m_Width     ;
            uint32_t                    m_Height    ;
            TextureInternalFormat       m_Format    ;
            bool                        m_BadW      ;
            bool                        m_BadH      ;              
    };
}