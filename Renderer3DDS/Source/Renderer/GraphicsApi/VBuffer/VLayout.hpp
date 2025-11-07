#pragma once 

#include <Defs.hpp>
#include <Core.hpp>
#include <vector>
#include "../Format/Format.hpp"

namespace MultiStation{

    /**
    * \author Dimitris Smyrnakis
	* \namespace MultiStation
	* \name VertexAttribute
	* \brief Enumeration of possible data types for vertex attributes
	* \details This is an enumeration that defines the various data types that can be used 
	* for vertex attributes in graphics rendering. Each type corresponds to a specific format and size,
	* as as outlined below:
	*   1. Type : The data type of the vertex attribute, represented by the ShaderDataType enumeration.
	*   2. Size : The size of the attribute in bytes.
	*   3. Packed : A boolean indicating whether the attribute is non packed in a sub-block within the RAM buffer or grouped with other attributes.
	*   4. Offs : If the attribute is packed, this indicates the offset within the buffer , otherwise indicates the offset relative to the group.
	*   5. Normallized : A boolean indicating whether the attribute values should be normalized or not.
    */
    struct VertexAttribute{
        ShaderDataType  Type; // Type of the Vertex attribute
        uint32_t        Size; // Size in bytes
        bool            Packed; // Is these attributes in a sub - block in RAM buffer or a set with the rest ?
        uint32_t        Offs; // if is Packed then what is the offset in the buffer 
		bool            Normallized; // Is the attribute normalized ?

        VertexAttribute( ShaderDataType type , bool Norm = true , bool packed = false);
        VertexAttribute(void) ;

        uint32_t GetComponentCount(void) const;
    };

    /**
     * \author Dimitris Smyrnakis.
	 * \details Attribute Group Struct , holds a group of packed attributes and their stride.
	 * If is only one attribute then it is not packed.
     */
	struct AttributeGroup {
		std::vector<VertexAttribute> Attributes;
		uint32_t                     Stride;
	};

    
    /**
	 * \author Dimitris Smyrnakis
	 * \brief Vertex Layout Class , holds information about the layout of vertex attributes in a vertex buffer.
	 * \details 
     */
    class VertexLayout{
        public:
            
            VertexLayout(void);

            void AddAttribute(const VertexAttribute& attribute);
			

            const std::vector<VertexAttribute>& GetAttributes(void) const ;
			const std::vector<AttributeGroup> GetAttributeGroups(void) const;

        private:
            std::vector<VertexAttribute> m_Attributes;
			std::vector<AttributeGroup>  m_AttributeGroups;
            uint32_t                     m_Stride    ;   
        private:
            void CalculateOffsetsAndStrides(void);
    };
}