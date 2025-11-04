#pragma once 

#include <Defs.hpp>
#include <glad/gl.h>
#include <vector>
#include "VLayout.hpp"

namespace Game{


    

    class VBuffer {
        public:
            VBuffer(void);
            ~VBuffer(void);

            void SetData(float* data , u32 size);

            void Bind(void) const ;
            void Unbind(void) const ;

            VertexLayout GetLayout(void) const;
            void SetLayout(const VertexLayout& layout) ;

        private:
            u32          m_BufferId ;
            VertexLayout m_Layout   ;
            float*       m_Data     ;
            u32          m_Size     ;
            
    };
}