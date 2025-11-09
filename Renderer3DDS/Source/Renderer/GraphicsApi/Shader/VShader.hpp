#pragma once

#include "../../../Defs.hpp"

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