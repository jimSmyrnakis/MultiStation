#pragma once 

#include "../../Definitions.h"
#include <vector>
#include "../Format/Format.hpp"

namespace MultiStation{

    /**
    * \author Dimitris Smyrnakis
	* \namespace MultiStation
	* \name VertexAttribute
	* \brief Enumeration of possible data types for vertex attributes
	* \details Defines various vertex data formats used in graphics rendering. 
	* The field 'Packed' indicates whether the attribute is part of a packed structure in memory (VertAttr1 , VertAttr2 ,...)
	* or a seperate block on it's own .
	* The field 'Normalized' indicates whether the attribute values should be normalized when accessed in shaders.
	* The filed 'Offs' indicates the offset in bytes within the packed structure if 'Packed' is true or nothing.
	* The field 'Size' indicates the size in bytes of the attribute data type.
	* The field 'Type' indicates the specific data type of the vertex attribute.
    */
    struct VertexAttribute{
        ShaderDataType  Type; // Type of the Vertex attribute
        uint32_t        Size; // Size in bytes
		bool            Packed; // Is these attributes in a group (true) or alone(false) (different chunk of memory) ?
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
	 * \details This class manages the layout of vertex attributes, including their types, sizes, offsets, and grouping.
     */
    class VertexLayout{
        public:
            
            VertexLayout(void) = default;
			~VertexLayout(void) = default;
			VertexLayout(const VertexLayout& other) = default;
			VertexLayout(VertexLayout&& other) noexcept = default;



            void AddAttribute(const VertexAttribute& attribute);
			

            const std::vector<VertexAttribute>& GetAttributes(void) const ;
			const std::vector<AttributeGroup> GetAttributeGroups(void) const;

			VertexLayout& operator=(const VertexLayout& other) = default;
			VertexLayout& operator=(VertexLayout&& other) noexcept = default;


        private:
            std::vector<VertexAttribute> m_Attributes;
			std::vector<AttributeGroup>  m_AttributeGroups;
        private:
            void CalculateOffsetsAndStrides(void);
    };
}