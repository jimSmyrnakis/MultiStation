#pragma once

#include <Defs.hpp>

namespace Game{
    class FShader{
        public:
            FShader(const char* source);
            ~FShader(void);

            u32 GetId(void) const ;

            bool HasCompiled(void) const;

        private:
            u32     m_Id;
            char*   m_Source;
            bool    m_HasCompile;
    };
}