#include "VBuffer.hpp"
#include <GL/glew.h>

namespace MultiStation{
    VBuffer::VBuffer(void){
		GLCALL( glGenVertexArrays(1, &m_VaoId) );
		m_iBufferData = nullptr;
		m_iBufferId = 0;
		m_iBufferAttribs = { 0,0,false };

    }

    VBuffer::~VBuffer(void){
		DestroyBuffers();
		GLCALL(glDeleteVertexArrays(1, &m_VaoId));
		DestroyIndexBuffer();
    }

    void VBuffer::DestroyBuffers(void) {
        for (auto id : m_BufferIds) {
            GLCALL(glDeleteBuffers(1, &id));
        }
		m_BufferIds.clear();
		m_Data.clear();
		m_Size.clear();

    }

    void VBuffer::SetData(void* data , uint32_t size, uint32_t index){
        ASSERT(size , "Vertex data have 0 size !!!");
        ASSERT(data , "nullptr for data is not valid");
		ASSERT(index < m_Layout.GetAttributeGroups().size(), "Index out of range for Vertex Buffer data set !");
        
		/*Take last one vertex Array Buffer and then change it */
		int32_t OldVao = 0;
		//get old vao
		GLCALL(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &OldVao));
		/*Take last binded Vertex Buffer and then change it */
		int32_t OldVBuf = 0;
		//get old vbo 
		GLCALL(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &OldVBuf));

		// bind our vao and vbo
		GLCALL(glBindVertexArray(m_VaoId));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferIds[index]));
		
		// set data
		AttributeGroup group = m_Layout.GetAttributeGroups().at(index);
		m_Data[index] = data;
		m_Size[index] = size;
		GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));


		// restore old vao and vbo
		GLCALL(glBindVertexArray(OldVao));
        GLCALL( glBindBuffer(GL_ARRAY_BUFFER , OldVBuf) );
    }

    void VBuffer::Bind(void) const {
		GLCALL(glBindVertexArray(m_VaoId));
    }

	void VBuffer::SetupVertexAttributes(void) const {
		

		// bind our vao 
		GLCALL(glBindVertexArray(m_VaoId));
		uint32_t location = 0;
		for (int i = 0; i < m_Layout.GetAttributeGroups().size(); i++) {
			AttributeGroup group = m_Layout.GetAttributeGroups().at(i);
			uint32_t bufferId = m_BufferIds.at(i);
			GLCALL(glBindBuffer(GL_ARRAY_BUFFER, bufferId));

			for (uint32_t index = 0; index < group.Attributes.size(); index++) {
				VertexAttribute& attribute = group.Attributes.at(index);
				GLCALL(glEnableVertexAttribArray(location));
				
				GLCALL(glVertexAttribPointer(
					location,
					attribute.GetComponentCount(),
					OpenGLAttributeDataTypeFromShaderDataType(attribute.Type),
					attribute.Normallized ? GL_TRUE : GL_FALSE,
					group.Stride,
					(const void*)(uintptr_t)(attribute.Offs)
				));
				location++;
			}
		}

	}

    void VBuffer::Unbind(void) const {
		// unbind our vao
		GLCALL(glBindVertexArray(0));
		GLCALL( glBindBuffer(GL_ARRAY_BUFFER , 0) ); 
	}

    const VertexLayout& VBuffer::GetLayout(void) const  { return m_Layout; }
    VertexLayout& VBuffer::GetLayout(void) { return m_Layout; }
    void VBuffer::SetLayout(const VertexLayout& layout) {
        m_Layout = layout; 
        DestroyBuffers();
		// Create buffers based on layout
		for (const auto& group : m_Layout.GetAttributeGroups()) {
			uint32_t bufferId;
			GLCALL(glGenBuffers(1, &bufferId));
			m_BufferIds.push_back(bufferId);
			m_Data.push_back(nullptr);
			m_Size.push_back(0);
		}

		// get old vao
		uint32_t oldvao = 0;
		GLCALL(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&oldvao));
		uint32_t oldvbo = 0;
		GLCALL(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&oldvbo));

		// setup vertex attributes
		SetupVertexAttributes();

		// restore old vao and vbo
		GLCALL(glBindVertexArray(oldvao));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, oldvbo));
    }

	void VBuffer::SetIndexBuffer(void* data, IBufferAttrib attrs) {
		ASSERT(data, "nullptr for index buffer data is not valid");
		DestroyIndexBuffer();
		m_iBufferData = data;
		m_iBufferAttribs = attrs;

		// Get Old Vao
		int32_t OldVao = 0;
		GLCALL(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &OldVao));
		// Get Old Index Buffer
		int32_t OldIBuf = 0;
		GLCALL(glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &OldIBuf));


		// Create and Initiallize Index Buffer
		GLCALL(glGenBuffers(1, &m_iBufferId));
		GLCALL(glBindVertexArray(m_VaoId));
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBufferId));
		GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, attrs.TypeSize * attrs.Count, data, GL_STATIC_DRAW));
		

		// Restore Old Vao and Index Buffer
		GLCALL(glBindVertexArray(OldVao));
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OldIBuf));

	}

	IBufferAttrib VBuffer::GetIndexBufferAttribs(void) const {
		return m_iBufferAttribs;
	}

	void* VBuffer::GetIndexBufferData(void) const {
		return m_iBufferData;
	}

	bool VBuffer::HasIndexBuffer(void) const {
		return m_iBufferId != 0;
	}

	void VBuffer::DestroyIndexBuffer(void) {
		if (HasIndexBuffer()) {
			
			GLCALL(glDeleteBuffers(1, &m_iBufferId));
			m_iBufferId = 0;
			m_iBufferData = nullptr;
			m_iBufferAttribs = { 0,0,false };
		}
	}
}