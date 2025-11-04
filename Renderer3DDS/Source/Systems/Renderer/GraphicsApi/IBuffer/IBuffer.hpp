#pragma once
#include <Defs.hpp>


namespace Game{
    class IBuffer{
        public:
            enum class Type{
                INT     ,
                UINT    ,
                SHORT   ,
                USHORT  ,
                BYTE    ,
                UBYTE   ,
                NONE
            };
        public:
            IBuffer(void);
            ~IBuffer(void);

            void SetData(u32* data , u32 count);
            void SetData(i32* data , u32 count);
            void SetData(u16* data , u32 count);
            void SetData(i16* data , u32 count);
            void SetData( u8* data , u32 count);
            void SetData( i8* data , u32 count);

            u32  GetCount(void) const;
            Type GetType(void) const;

            void Bind(void) const;
            void Unbind(void) const;

        private:
            u32     m_BufferId;
            void*   m_Data;
            u32     m_Size;
            Type    m_Type;

            

    };
}