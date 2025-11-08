#include "VLayout.hpp"
#include "../Init.hpp"
namespace MultiStation {
    
    #define ShaderDataTypeToOpenGLType ShaderDataTypeToOpenGLDataType

    /*
        ==================================================================================================================================
        ==============================================================Vertex Attribute====================================================
        ==================================================================================================================================
    */

    VertexAttribute::VertexAttribute( ShaderDataType type , bool Norm , bool packed)
        :   Type(type) , Size(0) , Offs(0) , Normallized(Norm) , Packed(packed) {
        Size = SizeOfShaderDataType(Type);

    }

	VertexAttribute::VertexAttribute(void) : Type(ShaderDataType::FLOAT), Size(SizeOfShaderDataType(ShaderDataType::FLOAT)),
        Offs(0), Normallized(true), Packed(false) {
	}

    uint32_t VertexAttribute::GetComponentCount(void) const{

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
    

    void VertexLayout::AddAttribute(const VertexAttribute& attribute){
        m_Attributes.push_back(attribute);
        if (attribute.Packed) {
			if (m_AttributeGroups.size() == 0 || !m_AttributeGroups.back().Attributes.back().Packed) {
				// Create a new group
				AttributeGroup newGroup;
				newGroup.Attributes.push_back(attribute);
				m_AttributeGroups.push_back(newGroup);
			}
			else {
				// Add to the existing group
				m_AttributeGroups.back().Attributes.push_back(attribute);
			}
		}
        else {
			// Non-packed attribute, create a new group for it
			AttributeGroup newGroup;
			newGroup.Attributes.push_back(attribute);
            m_AttributeGroups.push_back(newGroup);
        }

        CalculateOffsetsAndStrides();
    }

    void VertexLayout::CalculateOffsetsAndStrides(void){
        ASSERT(m_Attributes.size() , "Vertex Buffer Layout has not Vertex Attributes specified!");
		
        for (AttributeGroup& Group : m_AttributeGroups){
			uint32_t GroupOffset = 0;
            for (VertexAttribute& attr : Group.Attributes) {
                attr.Offs = GroupOffset;
                GroupOffset += attr.Size;
            }
			Group.Stride = GroupOffset;
        }
    }

    const std::vector<AttributeGroup> VertexLayout::GetAttributeGroups(void) const {
        return m_AttributeGroups;
    }

    
}