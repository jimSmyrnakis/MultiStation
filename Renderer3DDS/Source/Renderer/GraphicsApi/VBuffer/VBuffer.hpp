#pragma once 

#include <Defs.hpp>
#include <vector>
#include "VLayout.hpp"

namespace MultiStation{

    
	struct IBufferAttrib {
		uint32_t    TypeSize;
		uint32_t    Count;
        bool        Signed;
	};;

    class VBuffer {
        public:
            VBuffer(void);
            ~VBuffer(void);

            void SetData(void* data , uint32_t size , uint32_t index);

            void Bind(void) const ;
            void Unbind(void) const ;

			void SetIndexBuffer(void* data , IBufferAttrib attrs);

			IBufferAttrib GetIndexBufferAttribs(void) const;
			void* GetIndexBufferData(void) const;
			bool HasIndexBuffer(void) const;

            const VertexLayout& GetLayout(void) const;
            VertexLayout& GetLayout(void) ;
            void SetLayout(const VertexLayout& layout) ;

        private:
            void DestroyBuffers(void);
			void DestroyIndexBuffer(void);
			void SetupVertexAttributes(void) const;

        private:
			std::vector<uint32_t>   m_BufferIds     ; // per attribute group
            VertexLayout            m_Layout        ;
			std::vector<void*>      m_Data          ; // per attribute group
			std::vector<uint32_t>   m_Size          ; // per attribute group
			uint32_t 			    m_VaoId         ; // Vertex Array Object ID    
			uint32_t                m_iBufferId     ; // Index Buffer ID
			IBufferAttrib           m_iBufferAttribs; // Index Buffer Attributes
			void*                   m_iBufferData   ; // Index Buffer Data    
            
    };
}