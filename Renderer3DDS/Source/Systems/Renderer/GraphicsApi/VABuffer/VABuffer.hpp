#pragma once
#include <Defs.hpp>
#include "../VBuffer/VLayout.hpp"
#include "../VBuffer/VBuffer.hpp"
#include "../IBuffer/IBuffer.hpp"
#include <vector>

namespace Game{
    IBuffer& NullIBuffer(void);
    class VABuffer{
        public:
            VABuffer(void);
            ~VABuffer(void);

            void Bind(void) const ;
            void Unbind(void) const ;
            void AddVertexBuffer(const VBuffer& vo) ;
            void SetIndexBuffer(const IBuffer& io) ;
            IBuffer& GetIndexBuffer(void) const ;
            
        private:
            u32                    m_BufferId;
            std::vector<VBuffer*>  m_VBuffers;
            IBuffer*               m_IBuffer;
    };
}