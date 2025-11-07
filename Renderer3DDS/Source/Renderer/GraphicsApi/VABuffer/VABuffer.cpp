#include "VABuffer.hpp"
#include <GL/glew.h>

namespace MultiStation{

    IBuffer& NullIBuffer(void){
        static IBuffer nullibuffer;
        return nullibuffer;
    }

    VABuffer::VABuffer(void){
        m_IBuffer = nullptr;
        GLCALL( glGenVertexArrays(1 , &m_BufferId) );
    }

    VABuffer::~VABuffer(void){
        
        GLCALL( glDeleteBuffers(1 , &m_BufferId) );
    }

    void VABuffer::Bind(void) const {
        GLCALL( glBindVertexArray(m_BufferId) );
        
        for (auto el : m_VBuffers)
            el->Bind();
    

        if (m_IBuffer != nullptr)
            m_IBuffer->Bind();
    }

    void VABuffer::Unbind(void) const {
    
        for (auto el : m_VBuffers)
            el->Unbind();
        

        if (m_IBuffer != nullptr)
            m_IBuffer->Unbind();
            
        glBindVertexArray(0);
    }
    
    void VABuffer::AddVertexBuffer(const VBuffer& vo) {
        ASSERT(vo.GetLayout().GetAttributes().size() , "Vertex Buffer has no layout !");
		uint32_t old = 0;
        //get old vao
		GLCALL(glGetIntegerv(GL_VERTEX_ARRAY_BUFFER_BINDING, (GLint*)&old));

        GLCALL( glBindVertexArray(m_BufferId) );
        vo.Bind();
    

    
        m_VBuffers.push_back((VBuffer*)&vo);

		GLCALL(glBindVertexArray(old));

    }

    void VABuffer::SetIndexBuffer(const IBuffer& io){
        glBindVertexArray(m_BufferId);
        io.Bind();
        m_IBuffer = (IBuffer*)&io;
    }

    IBuffer& VABuffer::GetIndexBuffer(void) const {
        ASSERT(m_IBuffer , "Vertex Array Buffer (VABuffer) has no Index Buffer !!!");
        if (m_IBuffer == nullptr)
            return NullIBuffer();
        return *m_IBuffer;
    }

    
}