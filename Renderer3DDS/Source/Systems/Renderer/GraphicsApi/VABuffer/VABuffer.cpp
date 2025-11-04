#include "VABuffer.hpp"
#include <glad/gl.h>

namespace Game{

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
        GLCALL( glBindVertexArray(m_BufferId) );
        vo.Bind();
    
        u32 layout_index = 0;
    
        VertexAttribute attribute = vo.GetLayout().GetAttributes().at(layout_index);
    
        for (layout_index = 0 ; layout_index < vo.GetLayout().GetAttributes().size(); layout_index++){
    
            GLCALL( glEnableVertexAttribArray(layout_index) );
            attribute = vo.GetLayout().GetAttributes().at(layout_index);
            GLCALL( glVertexAttribPointer(
                layout_index ,
                attribute.GetComponentCount(),
                OpenGLAttributeDataTypeFromShaderDataType(attribute.Type) ,
                attribute.Normallized ? GL_FALSE : GL_TRUE ,
                vo.GetLayout().GetStride() ,
                (const void*)attribute.Offs 
            ) );
            
        }
    
        m_VBuffers.push_back((VBuffer*)&vo);

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