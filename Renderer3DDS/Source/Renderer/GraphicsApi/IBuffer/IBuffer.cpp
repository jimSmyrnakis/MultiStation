#include "IBuffer.hpp"
#include <GL/glew.h>

namespace MultiStation{
    IBuffer::IBuffer(void){
        m_Data = nullptr;
        m_Size = 0;
        m_Type = Type::NONE;
        GLCALL( glGenBuffers(1 , &m_BufferId) );
    }
    
    IBuffer::~IBuffer(void){
        GLCALL( glInvalidateBufferData(m_BufferId) );
        GLCALL( glDeleteBuffers(1 , &m_BufferId) );
    }

    void IBuffer::SetData(uint32_t* data , uint32_t count){
        if (count == 0){
            ASSERT(0 , "Index Buffer Data count is zero (0) !!!");
            return;
        }
        

        m_Data = data;
        m_Size = count * sizeof(uint32_t);
        m_Type = Type::UINT;

        
        int32_t old = 0;
        //GLCALL( glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER , GL_ELEMENT_ARRAY_BUFFER_BINDING , &old) );

        GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , m_BufferId) );
        GLCALL( glBufferData(GL_ELEMENT_ARRAY_BUFFER , m_Size , nullptr , GL_STATIC_DRAW) );
        void* map_ptr = nullptr;
        GLCALL( map_ptr = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER  , GL_WRITE_ONLY) );
        if (map_ptr == nullptr){
            GLCALL( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER , 0 , m_Size , data));
        } else {
            memcpy(map_ptr , data , m_Size);
            GLCALL( glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) );
        }

        //GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , old) );
    }

    void IBuffer::SetData(int32_t* data , uint32_t count){
        if (count == 0){
            ASSERT(0 , "Index Buffer Data count is zero (0) !!!");
            return;
        }
        

        m_Data = data;
        m_Size = count * sizeof(int32_t);
        m_Type = Type::INT;

        
        int32_t old = 0;
        //GLCALL( glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER , GL_ELEMENT_ARRAY_BUFFER_BINDING , &old) );

        GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , m_BufferId) );
        GLCALL( glBufferData(GL_ELEMENT_ARRAY_BUFFER , m_Size , nullptr , GL_STATIC_DRAW) );
        void* map_ptr = nullptr;
        GLCALL( map_ptr = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER , 0 , m_Size , GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT) );
        if (map_ptr == nullptr){
            GLCALL( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER , 0 , m_Size , data));
        } else {
            memcpy(map_ptr , data , m_Size);
            GLCALL( glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) );
        }

        GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , old) );
    }

    void IBuffer::SetData(uint16_t* data , uint32_t count){
        if (count == 0){
            ASSERT(0 , "Index Buffer Data count is zero (0) !!!");
            return;
        }
        

        m_Data = data;
        m_Size = count * sizeof(uint16_t);
        m_Type = Type::USHORT;

        
        int32_t old = 0;
        //GLCALL( glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER , GL_ELEMENT_ARRAY_BUFFER_BINDING , &old) );

        GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , m_BufferId) );
        GLCALL( glBufferData(GL_ELEMENT_ARRAY_BUFFER , m_Size , nullptr , GL_STATIC_DRAW) );
        void* map_ptr = nullptr;
        GLCALL( map_ptr = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER , 0 , m_Size , GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT) );
        if (map_ptr == nullptr){
            GLCALL( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER , 0 , m_Size , data));
        } else {
            memcpy(map_ptr , data , m_Size);
            GLCALL( glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) );
        }

        GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , old) );
    }

    void IBuffer::SetData(int16_t* data , uint32_t count){
        if (count == 0){
            ASSERT(0 , "Index Buffer Data count is zero (0) !!!");
            return;
        }
        

        m_Data = data;
        m_Size = count * sizeof(int16_t);
        m_Type = Type::SHORT;

        
        int32_t old = 0;
        //GLCALL( glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER , GL_ELEMENT_ARRAY_BUFFER_BINDING , &old) );

        GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , m_BufferId) );
        GLCALL( glBufferData(GL_ELEMENT_ARRAY_BUFFER , m_Size , nullptr , GL_STATIC_DRAW) );
        void* map_ptr = nullptr;
        GLCALL( map_ptr = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER , 0 , m_Size , GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT) );
        if (map_ptr == nullptr){
            GLCALL( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER , 0 , m_Size , data));
        } else {
            memcpy(map_ptr , data , m_Size);
            GLCALL( glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) );
        }

        GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , old) );
    }

    void IBuffer::SetData( uint8_t* data , uint32_t count){
        if (count == 0){
            ASSERT(0 , "Index Buffer Data count is zero (0) !!!");
            return;
        }
        

        m_Data = data;
        m_Size = count * sizeof(uint8_t);
        m_Type = Type::UBYTE;

        
        int32_t old = 0;
        //GLCALL( glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER , GL_ELEMENT_ARRAY_BUFFER_BINDING , &old) );

        GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , m_BufferId) );
        GLCALL( glBufferData(GL_ELEMENT_ARRAY_BUFFER , m_Size , nullptr , GL_STATIC_DRAW) );
        void* map_ptr = nullptr;
        GLCALL( map_ptr = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER , 0 , m_Size , GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT) );
        if (map_ptr == nullptr){
            GLCALL( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER , 0 , m_Size , data));
        } else {
            memcpy(map_ptr , data , m_Size);
            GLCALL( glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) );
        }

        //GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , old) );
    }

    void IBuffer::SetData( int8_t* data , uint32_t count){
        if (count == 0){
            ASSERT(0 , "Index Buffer Data count is zero (0) !!!");
            return;
        }
        

        m_Data = data;
        m_Size = count * sizeof(int8_t);
        m_Type = Type::BYTE;

        
        int32_t old = 0;
        //GLCALL( glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER , GL_ELEMENT_ARRAY_BUFFER_BINDING , &old) );

        GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , m_BufferId) );
        GLCALL( glBufferData(GL_ELEMENT_ARRAY_BUFFER , m_Size , nullptr , GL_STATIC_DRAW) );
        void* map_ptr = nullptr;
        GLCALL( map_ptr = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER , 0 , m_Size , GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT) );
        if (map_ptr == nullptr){
            GLCALL( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER , 0 , m_Size , data));
        } else {
            memcpy(map_ptr , data , m_Size);
            GLCALL( glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) );
        }

        //GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , old) );
    }

    void IBuffer::Bind(void)    const { GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , m_BufferId) ); }

    void IBuffer::Unbind(void)  const { GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , 0         ) ); }

    uint32_t  IBuffer::GetCount(void) const { return m_Size / sizeof(uint32_t); }

    IBuffer::Type IBuffer::GetType(void) const { return m_Type; }
}