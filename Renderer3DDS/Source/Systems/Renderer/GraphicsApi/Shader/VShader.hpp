#pragma once

#include <Defs.hpp>

namespace Game{
    class VShader{
        public:
            VShader(const char* source);
            ~VShader(void);

            u32 GetId(void) const ;

            bool HasCompiled(void) const;

        private:
            u32     m_Id;
            char*   m_Source;
            bool    m_HasCompile;
    };
}