#pragma once

#include <stdint.h>
#include <stddef.h>
#include <Platform.hpp>
namespace MultiStation{
    class VShader{
        public:
            VShader(const char* source);
            ~VShader(void);

            uint32_t GetId(void) const ;

            bool HasCompiled(void) const;

        private:
            uint32_t     m_Id;
            char*   m_Source;
            bool    m_HasCompile;
    };
}