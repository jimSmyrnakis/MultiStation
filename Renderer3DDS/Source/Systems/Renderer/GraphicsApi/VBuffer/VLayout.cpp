#include "VLayout.hpp"

namespace Game{
    
    #define ShaderDataTypeToOpenGLType ShaderDataTypeToOpenGLDataType

    /*
        ==================================================================================================================================
        ==============================================================Vertex Attribute====================================================
        ==================================================================================================================================
    */

    VertexAttribute::VertexAttribute( ShaderDataType type , bool Norm)
        :   Type(type) , Size(0) , Offs(0) , Normallized(Norm) {
        Size = SizeOfShaderDataType(Type);

    }

    u32 VertexAttribute::GetComponentCount(void) const{

        switch(Type){
            case ShaderDataType::FLOAT      : return 1  ;
            case ShaderDataType::VEC2F      : return 2  ;
            case ShaderDataType::VEC3F      : return 3  ;
            case ShaderDataType::VEC4F      : return 4  ;
            case ShaderDataType::MAT3F_T    : return 3*3;
            case ShaderDataType::MAT4F_T    : return 4*4;

            case ShaderDataType::INT        : return 1  ;
            case ShaderDataType::VEC2I      : return 2  ;
            case ShaderDataType::VEC3I      : return 3  ;
            case ShaderDataType::VEC4I      : return 4  ;

            case ShaderDataType::UINT       : return 1  ;
            case ShaderDataType::VEC2UI     : return 2  ;
            case ShaderDataType::VEC3UI     : return 3  ;
            case ShaderDataType::VEC4UI     : return 4  ;
        };
        ASSERT(0 , "Unknown Shader Data Type !");
        return 0;
    }

    /*
        ==================================================================================================================================
        ==============================================================Vertex Layout=======================================================
        ==================================================================================================================================
    */
    
    VertexLayout::VertexLayout(void){
        m_Stride = 0;
    }
    
    const std::vector<VertexAttribute>& VertexLayout::GetAttributes(void) const { return m_Attributes; }
    
    u32 VertexLayout::GetStride(void) const { return m_Stride; }

    void VertexLayout::AddAttribute(const VertexAttribute& attribute){
        m_Attributes.push_back(attribute);
        CalculateOffsetsAndStride();
    }

    void VertexLayout::CalculateOffsetsAndStride(void){
        ASSERT(m_Attributes.size() , "Vertex Buffer Layout has not Vertex Attributes specified!");
        uint32_t Offset = 0;
        for (VertexAttribute& attribute : m_Attributes){
            attribute.Offs = Offset;
            Offset += attribute.Size;
        }
        m_Stride = Offset;
    }
    
}