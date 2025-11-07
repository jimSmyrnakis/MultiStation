#pragma once

#include <Defs.hpp>


namespace MultiStation {
    enum class TextureInternalFormat{
        RGB4        ,
        RGB8        ,
        RGB16       ,

        RGB4UI      ,
        RGB8UI      ,
        RGB16UI     ,
        RGB32UI     ,

        RGB4I       ,
        RGB8I       ,
        RGB16I      ,
        RGB32I      ,

        

        RGBA4       ,
        RGBA8       ,
        RGBA16      ,

        RGBA4UI     ,
        RGBA8UI     ,
        RGBA16UI    ,
        RGBA32UI    ,

        RGBA4I      ,
        RGBA8I      ,
        RGBA16I     ,
        RGBA32I     ,


        RGBA16F     ,
        RGBA32F     ,
        RGB16F      ,
        RGB32F      
    };
    
    uint32_t GetOpenGLInternalTextureFormat(TextureInternalFormat  format);

    
    enum class TextureExternalFormat{
        RGB4            ,
        RGB8            ,
        RGB16           ,
        RGB32           ,

        RGBA4           ,
        RGBA8           ,
        RGBA16          ,
        RGBA32          ,

        RGB16F_RAW      ,
        RGB32F_RAW      ,

        RGBA16F_RAW     ,
        RGBA32F_RAW     ,

        RGB4U_RAW       ,
        RGB8U_RAW       ,
        RGB16U_RAW      ,
        RGB32U_RAW      ,

        RGBA4U_RAW      ,
        RGBA8U_RAW      ,
        RGBA16U_RAW     ,
        RGBA32U_RAW     ,

        RGB4I_RAW       ,
        RGB8I_RAW       ,
        RGB16I_RAW      ,
        RGB32I_RAW      ,

        RGBA4I_RAW      ,
        RGBA8I_RAW      ,
        RGBA16I_RAW     ,
        RGBA32I_RAW     
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