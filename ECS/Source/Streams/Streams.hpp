#pragma once

#include <cstddef>
namespace MultiStation {


        class IReadStream
        {
        public:
            virtual ~IReadStream() = default;

            virtual bool Open(void) = 0;
            // Returns the number of bytes actually read.
            virtual size_t Read(void* dest , size_t size) = 0;

            virtual bool IsOpen(void) const = 0;
        };

        class IWriteStream
        {
        public:
            virtual ~IWriteStream() = default;
            
            virtual bool Open(void) = 0;

            // Returns the number of bytes actually written.
            virtual size_t Write(const void* src , size_t size) = 0;

            virtual void Flush() = 0;

            virtual bool IsOpen(void) const = 0;
        };
    

}
