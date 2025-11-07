#pragma once
#include <Defs.hpp>


namespace MultiStation{
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

            void SetData(uint32_t* data , uint32_t count);
            void SetData(int32_t* data , uint32_t count);
            void SetData(uint16_t* data , uint32_t count);
            void SetData(int16_t* data , uint32_t count);
            void SetData( uint8_t* data , uint32_t count);
            void SetData( int8_t* data , uint32_t count);

            uint32_t  GetCount(void) const;
            Type GetType(void) const;

            void Bind(void) const;
            void Unbind(void) const;

        private:
            uint32_t     m_BufferId;
            void*   m_Data;
            uint32_t     m_Size;
            Type    m_Type;

            

    };
}