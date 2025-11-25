#pragma once

#include "../../../Defs.hpp"


namespace MultiStation {
    // Supported Internal Textures Formats GPU side
    enum class TextureInternalFormat{
        R8 ,
        
        RGB8        ,
        RGB16       ,
        
        RGBA8       ,
        RGBA16      ,

        RGBA16F     ,
        RGBA32F     ,

        RGB16F      ,
        RGB32F      
    };
    uint32_t GetOpenGLInternalTextureFormat(TextureInternalFormat  format);
    

    
    enum class TextureExternalFormat{
        RGB8            ,
        RGB16           ,

        RGBA8           ,
        RGBA16          ,

        RGB16F_RAW      ,
        RGB32F_RAW      ,

        RGBA16F_RAW     ,
        RGBA32F_RAW     ,



    };

    uint32_t GetOpenGLExternalTextureFormat(TextureExternalFormat  format);
    uint32_t GetOpenGLExternalTextureType(TextureExternalFormat format );






















    enum class DepthFormat{
        DEPTH16 , DEPTH24 , DEPTH32
    };

    uint32_t GetOpenGLDepthFormat(DepthFormat format);

    enum class Filter{
        LINEAR , NEAREST , BILINEAR
    };

    uint32_t GetOpenGLFilter(Filter filter);

    enum class ShaderDataType{
        FLOAT       ,
        VEC2F       ,
        VEC3F       ,
        VEC4F       ,
        MAT3F_T     ,
        MAT4F_T     ,
        INT         ,
        VEC2I       ,
        VEC3I       ,
        VEC4I       ,
        UINT        ,
        VEC2UI      ,
        VEC3UI      ,
        VEC4UI      ,
        SAMPLER_2D  ,
        USAMPLER_2D ,
        ISAMPLER_2D ,

        NONE
    };

    uint32_t        SizeOfShaderDataType            (ShaderDataType type    );
    uint32_t        ShaderDataTypeToOpenGLDataType  (ShaderDataType type    );
    ShaderDataType  OpenGLDataTypeToShaderDataType  (uint32_t       ogltype );

    // Special for attributes
    uint32_t OpenGLAttributeDataTypeFromShaderDataType(ShaderDataType type    );

}