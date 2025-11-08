#include "Format.hpp"
#include <GL/glew.h>
#include "../Init.hpp"


namespace MultiStation {

    uint32_t GetOpenGLInternalTextureFormat(TextureInternalFormat  format){
        
        switch (format){
            case TextureInternalFormat::RGB4    :   return GL_RGB4      ;
            case TextureInternalFormat::RGB8    :   return GL_RGB8      ;
            case TextureInternalFormat::RGB16   :   return GL_RGB16     ;

            case TextureInternalFormat::RGB32UI :   return GL_RGB32UI   ;
            case TextureInternalFormat::RGB16F  :   return GL_RGB16F    ;
            case TextureInternalFormat::RGB32F  :   return GL_RGB32F    ;
            case TextureInternalFormat::RGBA4   :   return GL_RGBA4     ;
            case TextureInternalFormat::RGBA8   :   return GL_RGBA8     ;
            case TextureInternalFormat::RGBA16  :   return GL_RGBA16    ;
            case TextureInternalFormat::RGBA32UI:   return GL_RGBA32UI  ;
            case TextureInternalFormat::RGBA16F :   return GL_RGBA16F   ;
            case TextureInternalFormat::RGBA32F :   return GL_RGBA32F   ;

            default:
                ASSERT(0 , "Undefined Internal Texture Format Type !!!");
        }
        return 0;
    }

    uint32_t GetOpenGLExternalTextureFormat(TextureExternalFormat  format){
        switch (format){
            case TextureExternalFormat::RGB4        :   return GL_RGB           ;
            case TextureExternalFormat::RGB8        :   return GL_RGB           ;
            case TextureExternalFormat::RGB16       :   return GL_RGB           ;
            case TextureExternalFormat::RGB32       :   return GL_RGB           ;

            case TextureExternalFormat::RGBA4       :   return GL_RGBA          ;
            case TextureExternalFormat::RGBA8       :   return GL_RGBA          ;
            case TextureExternalFormat::RGBA16      :   return GL_RGBA          ;
            case TextureExternalFormat::RGBA32      :   return GL_RGBA          ;

            case TextureExternalFormat::RGB16F_RAW  :   return GL_RGB           ;
            case TextureExternalFormat::RGB32F_RAW  :   return GL_RGB           ;

            case TextureExternalFormat::RGBA16F_RAW :   return GL_RGBA          ;
            case TextureExternalFormat::RGBA32F_RAW :   return GL_RGBA          ;

            case TextureExternalFormat::RGB4U_RAW   :   return GL_RGB_INTEGER   ;
            case TextureExternalFormat::RGB8U_RAW   :   return GL_RGB_INTEGER   ;
            case TextureExternalFormat::RGB16U_RAW  :   return GL_RGB_INTEGER   ;
            case TextureExternalFormat::RGB32U_RAW  :   return GL_RGB_INTEGER   ;

            case TextureExternalFormat::RGBA4U_RAW  :   return GL_RGBA_INTEGER  ;
            case TextureExternalFormat::RGBA8U_RAW  :   return GL_RGBA_INTEGER  ;
            case TextureExternalFormat::RGBA16U_RAW :   return GL_RGBA_INTEGER  ;
            case TextureExternalFormat::RGBA32U_RAW :   return GL_RGBA_INTEGER  ;

            case TextureExternalFormat::RGB4I_RAW   :   return GL_RGB_INTEGER   ;
            case TextureExternalFormat::RGB8I_RAW   :   return GL_RGB_INTEGER   ;
            case TextureExternalFormat::RGB16I_RAW  :   return GL_RGB_INTEGER   ;
            case TextureExternalFormat::RGB32I_RAW  :   return GL_RGB_INTEGER   ;

            case TextureExternalFormat::RGBA4I_RAW  :   return GL_RGBA_INTEGER  ;
            case TextureExternalFormat::RGBA8I_RAW  :   return GL_RGBA_INTEGER  ;
            case TextureExternalFormat::RGBA16I_RAW :   return GL_RGBA_INTEGER  ;
            case TextureExternalFormat::RGBA32I_RAW :   return GL_RGBA_INTEGER  ;

            default:
                ASSERT(0 , "Undefined Internal Texture Format Type !!!");
        }
        return 0;
    }

    uint32_t GetOpenGLExternalTextureType(TextureExternalFormat format ){
        switch (format){
            case TextureExternalFormat::RGB8        :   return GL_UNSIGNED_BYTE     ;
            case TextureExternalFormat::RGB16       :   return GL_UNSIGNED_SHORT    ;
            case TextureExternalFormat::RGB32       :   return GL_UNSIGNED_INT      ;

            case TextureExternalFormat::RGBA8       :   return GL_UNSIGNED_BYTE     ;
            case TextureExternalFormat::RGBA16      :   return GL_UNSIGNED_SHORT    ;
            case TextureExternalFormat::RGBA32      :   return GL_UNSIGNED_INT      ;

            case TextureExternalFormat::RGB16F_RAW  :   return GL_HALF_FLOAT        ;
            case TextureExternalFormat::RGB32F_RAW  :   return GL_FLOAT             ;

            case TextureExternalFormat::RGBA16F_RAW :   return GL_HALF_FLOAT        ;
            case TextureExternalFormat::RGBA32F_RAW :   return GL_FLOAT             ;

            case TextureExternalFormat::RGB8U_RAW   :   return GL_UNSIGNED_BYTE     ;
            case TextureExternalFormat::RGB16U_RAW  :   return GL_UNSIGNED_SHORT    ;
            case TextureExternalFormat::RGB32U_RAW  :   return GL_UNSIGNED_INT      ;

            case TextureExternalFormat::RGBA8U_RAW  :   return GL_UNSIGNED_BYTE     ;
            case TextureExternalFormat::RGBA16U_RAW :   return GL_UNSIGNED_SHORT    ;
            case TextureExternalFormat::RGBA32U_RAW :   return GL_UNSIGNED_INT      ;

            default:
                ASSERT(0 , "Undefined Internal Texture Format Type !!!");
        }
        return 0;
    }

    

    uint32_t GetOpenGLDepthFormat(DepthFormat format){
        switch(format){
            case DepthFormat::DEPTH16 : return GL_DEPTH_COMPONENT16;
            case DepthFormat::DEPTH24 : return GL_DEPTH_COMPONENT24;
            case DepthFormat::DEPTH32 : return GL_DEPTH_COMPONENT32;
            default:
                ASSERT(0 , "Undefind Depth Foramt Type !!!");
                return 0;
        }
        return 0;
    }


    uint32_t GetOpenGLFilter(Filter filter){
        switch (filter){
            case Filter::LINEAR : return GL_LINEAR;
            case Filter::NEAREST: return GL_NEAREST;
            default:
                ASSERT(0 , "Undefined Filter Type !!!");
                return 0;
        }
        return 0;
    }


    uint32_t SizeOfShaderDataType(ShaderDataType type){
        switch(type){
            case ShaderDataType::FLOAT      :   return sizeof(float)            ;
            case ShaderDataType::VEC2F      :   return sizeof(float) * 2        ;
            case ShaderDataType::VEC3F      :   return sizeof(float) * 3        ;
            case ShaderDataType::VEC4F      :   return sizeof(float) * 4        ;
            case ShaderDataType::MAT3F_T    :   return sizeof(float) * 3 * 3    ;
            case ShaderDataType::MAT4F_T    :   return sizeof(float) * 4 * 4    ;

            case ShaderDataType::INT        :   return sizeof(int32_t)              ;
            case ShaderDataType::VEC2I      :   return sizeof(int32_t) * 2          ;
            case ShaderDataType::VEC3I      :   return sizeof(int32_t) * 3          ;
            case ShaderDataType::VEC4I      :   return sizeof(int32_t) * 4          ;

            case ShaderDataType::UINT       :   return sizeof(uint32_t)              ;
            case ShaderDataType::VEC2UI     :   return sizeof(uint32_t) * 2          ;
            case ShaderDataType::VEC3UI     :   return sizeof(uint32_t) * 3          ;
            case ShaderDataType::VEC4UI     :   return sizeof(uint32_t) * 4          ;

            case ShaderDataType::SAMPLER_2D :   return sizeof(int32_t)              ;
            case ShaderDataType::ISAMPLER_2D:   return sizeof(int32_t)              ;
            case ShaderDataType::USAMPLER_2D:   return sizeof(int32_t)              ;


            default : ASSERT(0 , "Undefined Shader Data Type !!!"); return 0;
        }
        return 0;
    }

    uint32_t ShaderDataTypeToOpenGLDataType(ShaderDataType type){
        switch(type){
            case ShaderDataType::FLOAT      :   return GL_FLOAT                     ;
            case ShaderDataType::VEC2F      :   return GL_FLOAT_VEC2                ;
            case ShaderDataType::VEC3F      :   return GL_FLOAT_VEC3                ;
            case ShaderDataType::VEC4F      :   return GL_FLOAT_VEC4                ;
            case ShaderDataType::MAT3F_T    :   return GL_FLOAT_MAT3                ;
            case ShaderDataType::MAT4F_T    :   return GL_FLOAT_MAT4                ;

            case ShaderDataType::INT        :   return GL_INT                       ;
            case ShaderDataType::VEC2I      :   return GL_INT_VEC2                  ;
            case ShaderDataType::VEC3I      :   return GL_INT_VEC3                  ;
            case ShaderDataType::VEC4I      :   return GL_INT_VEC4                  ;

            case ShaderDataType::UINT       :   return GL_UNSIGNED_INT              ;
            case ShaderDataType::VEC2UI     :   return GL_UNSIGNED_INT_VEC2         ;
            case ShaderDataType::VEC3UI     :   return GL_UNSIGNED_INT_VEC3         ;
            case ShaderDataType::VEC4UI     :   return GL_UNSIGNED_INT_VEC4         ;

            case ShaderDataType::SAMPLER_2D :   return GL_SAMPLER_2D                ;
            case ShaderDataType::ISAMPLER_2D:   return GL_INT_SAMPLER_2D            ;
            case ShaderDataType::USAMPLER_2D:   return GL_UNSIGNED_INT_SAMPLER_2D   ;

            default : ASSERT(0 , "Undefined Shader Data Type !!!"); return 0;
        }
        return 0;
    }

    

    ShaderDataType OpenGLDataTypeToShaderDataType(uint32_t oglType){
        switch(oglType){
            case GL_FLOAT                   :   return  ShaderDataType::FLOAT               ;
            case GL_FLOAT_VEC2              :   return  ShaderDataType::VEC2F               ;
            case GL_FLOAT_VEC3              :   return  ShaderDataType::VEC3F               ;
            case GL_FLOAT_VEC4              :   return  ShaderDataType::VEC4F               ;
            case GL_FLOAT_MAT3              :   return  ShaderDataType::MAT3F_T             ;
            case GL_FLOAT_MAT4              :   return  ShaderDataType::MAT4F_T             ;

            case GL_INT                     :   return  ShaderDataType::INT                 ;
            case GL_INT_VEC2                :   return  ShaderDataType::VEC2I               ;
            case GL_INT_VEC3                :   return  ShaderDataType::VEC3I               ;
            case GL_INT_VEC4                :   return  ShaderDataType::VEC4I               ;

            case GL_UNSIGNED_INT            :   return  ShaderDataType::UINT                ;
            case GL_UNSIGNED_INT_VEC2       :   return  ShaderDataType::VEC2UI              ;
            case GL_UNSIGNED_INT_VEC3       :   return  ShaderDataType::VEC3UI              ;
            case GL_UNSIGNED_INT_VEC4       :   return  ShaderDataType::VEC4UI              ;
            
            case GL_SAMPLER_2D              :   return  ShaderDataType::SAMPLER_2D          ;
            case GL_INT_SAMPLER_2D          :   return  ShaderDataType::ISAMPLER_2D         ;
            case GL_UNSIGNED_INT_SAMPLER_2D :   return  ShaderDataType::USAMPLER_2D         ;

            default : ASSERT(0 , "Undefined Shader Data Type !!!"); return ShaderDataType::NONE;
        }
        return ShaderDataType::NONE;
    }

    uint32_t OpenGLAttributeDataTypeFromShaderDataType(ShaderDataType type    ){
        switch(type){
            case ShaderDataType::FLOAT      :   return GL_FLOAT                     ;
            case ShaderDataType::VEC2F      :   return GL_FLOAT                     ;
            case ShaderDataType::VEC3F      :   return GL_FLOAT                     ;
            case ShaderDataType::VEC4F      :   return GL_FLOAT                     ;
            case ShaderDataType::MAT3F_T    :   return GL_FLOAT                     ;
            case ShaderDataType::MAT4F_T    :   return GL_FLOAT                     ;

            case ShaderDataType::INT        :   return GL_INT                       ;
            case ShaderDataType::VEC2I      :   return GL_INT                       ;
            case ShaderDataType::VEC3I      :   return GL_INT                       ;
            case ShaderDataType::VEC4I      :   return GL_INT                       ;

            case ShaderDataType::UINT       :   return GL_UNSIGNED_INT              ;
            case ShaderDataType::VEC2UI     :   return GL_UNSIGNED_INT               ;
            case ShaderDataType::VEC3UI     :   return GL_UNSIGNED_INT               ;
            case ShaderDataType::VEC4UI     :   return GL_UNSIGNED_INT               ;


            default : ASSERT(0 , "Undefined Attribute Data Type !!!"); return 0;
        }
        return 0;
    }
}