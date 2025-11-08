#include "Texture2DResolution.hpp"

#include <GL/glew.h>

namespace MultiStation{
    Texture2DResolution::Texture2DResolution(const uint32_t& width  , const uint32_t& height  , const TextureInternalFormat& format ){
        SetWidth(width);
        SetHeight(height);
        SetFormat(format);
        m_BadW = m_BadH =  false;
    }

    void Texture2DResolution::SetWidth (const uint32_t&                   width ){
        if (width > GetMaxWidth()){
            m_Width = GetMaxWidth();
            m_BadW = true;
            ASSERT(0 , "Width Greater that the maximum one !!!");
            return;
        }
        m_BadW = false;
        m_Width = width;

    }
    void Texture2DResolution::SetHeight(const uint32_t&                   height){
        if (height > GetMaxHeight()){
            m_Height = GetMaxHeight();
            m_BadH = true;
            ASSERT(0 , "Height Greater that the maximum one !!!");
            return;
        }
        m_BadH = false;
        m_Height = height;
    }
    void Texture2DResolution::SetFormat(const TextureInternalFormat& format){
        m_Format = format;
    }

    uint32_t                   Texture2DResolution::GetWidth (void) const{
        ASSERT(!m_BadW , "Width has set to the maximum value ");
        return m_Width;
    }
    uint32_t                   Texture2DResolution::GetHeight(void) const{
        ASSERT(!m_BadH , "Height has set to the maximum value ");
        return m_Height;
    }
    TextureInternalFormat Texture2DResolution::GetFormat(void) const{
        return m_Format;
    }

    //TODO - Ask opengl implemantation specific about the maximum height and width 
    uint32_t Texture2DResolution::GetMaxWidth(void)  { return 1024 * 8 ; }
    uint32_t Texture2DResolution::GetMaxHeight(void) { return 1024 * 8 ; }
}