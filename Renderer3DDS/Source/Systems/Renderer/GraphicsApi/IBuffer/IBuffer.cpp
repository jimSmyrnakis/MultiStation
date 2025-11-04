#include "IBuffer.hpp"
#include <glad/gl.h>
#include <memory.h>

namespace Game{
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

    void IBuffer::SetData(u32* data , u32 count){
        if (count == 0){
            ASSERT(0 , "Index Buffer Data count is zero (0) !!!");
            return;
        }
        

        m_Data = data;
        m_Size = count * sizeof(u32);
        m_Type = Type::UINT;

        
        i32 old = 0;
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

    void IBuffer::SetData(i32* data , u32 count){
        if (count == 0){
            ASSERT(0 , "Index Buffer Data count is zero (0) !!!");
            return;
        }
        

        m_Data = data;
        m_Size = count * sizeof(i32);
        m_Type = Type::INT;

        
        i32 old = 0;
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

    void IBuffer::SetData(u16* data , u32 count){
        if (count == 0){
            ASSERT(0 , "Index Buffer Data count is zero (0) !!!");
            return;
        }
        

        m_Data = data;
        m_Size = count * sizeof(u16);
        m_Type = Type::USHORT;

        
        i32 old = 0;
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

    void IBuffer::SetData(i16* data , u32 count){
        if (count == 0){
            ASSERT(0 , "Index Buffer Data count is zero (0) !!!");
            return;
        }
        

        m_Data = data;
        m_Size = count * sizeof(i16);
        m_Type = Type::SHORT;

        
        i32 old = 0;
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

    void IBuffer::SetData( u8* data , u32 count){
        if (count == 0){
            ASSERT(0 , "Index Buffer Data count is zero (0) !!!");
            return;
        }
        

        m_Data = data;
        m_Size = count * sizeof(u8);
        m_Type = Type::UBYTE;

        
        i32 old = 0;
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

    void IBuffer::SetData( i8* data , u32 count){
        if (count == 0){
            ASSERT(0 , "Index Buffer Data count is zero (0) !!!");
            return;
        }
        

        m_Data = data;
        m_Size = count * sizeof(i8);
        m_Type = Type::BYTE;

        
        i32 old = 0;
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

    u32  IBuffer::GetCount(void) const { return m_Size / sizeof(u32); }

    IBuffer::Type IBuffer::GetType(void) const { return m_Type; }
}