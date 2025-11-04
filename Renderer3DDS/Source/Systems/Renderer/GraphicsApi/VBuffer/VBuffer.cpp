#include "VBuffer.hpp"
#include <memory.h>

namespace Game{
    VBuffer::VBuffer(void){
        GLCALL( glGenBuffers(1 , &m_BufferId) );
    }

    VBuffer::~VBuffer(void){
        GLCALL( glDeleteBuffers(1 , &m_BufferId) );
    }

    void VBuffer::SetData(float* data , u32 size){
        ASSERT(size , "Vertex data have 0 size !!!");
        ASSERT(data , "nullptr for data is not valid");
        m_Data = data;
        m_Size = size;
        /*Take last binded Vertex Buffer and then change it ?*/
        i32 OldArrBuf = 0;
        
        //GLCALL( glGetBufferParameteriv(GL_ARRAY_BUFFER , GL_ARRAY_BUFFER_BINDING , &OldArrBuf) );
        
        GLCALL( glBindBuffer(GL_ARRAY_BUFFER , m_BufferId) );
        GLCALL( glBufferData(GL_ARRAY_BUFFER , m_Size , nullptr , GL_STATIC_DRAW) );
        void* map_ptr;
        GLCALL( map_ptr = glMapBuffer(GL_ARRAY_BUFFER  , GL_WRITE_ONLY) );
        if (map_ptr == nullptr){
            ASSERT(0 , "Buffer Mapp memory failed !!!");
            GLCALL( glBufferSubData(GL_ARRAY_BUFFER , 0 , m_Size , data) );
        }
        else{
            memcpy(map_ptr , data , size);
            GLCALL( glUnmapBuffer(GL_ARRAY_BUFFER) );
        }
        
        GLCALL( glBindBuffer(GL_ARRAY_BUFFER , OldArrBuf) );
    }

    void VBuffer::Bind(void) const {  GLCALL( glBindBuffer(GL_ARRAY_BUFFER , m_BufferId) ); }
    void VBuffer::Unbind(void) const { GLCALL( glBindBuffer(GL_ARRAY_BUFFER , 0) ); }

    VertexLayout VBuffer::GetLayout(void) const  { return m_Layout; }
    void VBuffer::SetLayout(const VertexLayout& layout) { m_Layout = layout; }
}