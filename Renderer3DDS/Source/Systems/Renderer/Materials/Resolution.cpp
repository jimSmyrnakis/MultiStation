#include "Material.hpp"

namespace Game{
    Material2D::Resolution::Resolution(const u32& width , const u32& height , const Format& format ){
        SetWidth (width );
        SetHeight(height);
        SetFormat(format);
    }

    void Material2D::Resolution::SetFormat(const Format& format){
        m_Format = format;
    }
    void Material2D::Resolution::SetWidth (const u32& width    ){
        m_Width = width;
    }
    void Material2D::Resolution::SetHeight(const u32& height   ){
        m_Height = height;
    }

    Material2D::Format  Material2D::Resolution::GetFormat(void) const { return m_Format; }
    u32                 Material2D::Resolution::GetWidth (void) const { return m_Width ; }
    u32                 Material2D::Resolution::GetHeight(void) const { return m_Height; }
}