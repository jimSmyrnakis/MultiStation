#pragma once

#include "../../Definitions.h"

namespace MultiStation{
    class FShader{
        public:
            FShader(const char* source);
            ~FShader(void);

            uint32_t GetId(void) const ;

            bool HasCompiled(void) const;

        private:
            uint32_t     m_Id;
            char*   m_Source;
            bool    m_HasCompile;
    };
}