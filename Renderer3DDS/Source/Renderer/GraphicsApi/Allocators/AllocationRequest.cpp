#include "AllocationRequest.hpp"

namespace Game{
    AllocationRequest::AllocationRequest(u32 index , u32 request_size , u32 real_size , void* mapped_ptr , RequestStatus status , u32 offset ){
        m_Data              = mapped_ptr    ;
        m_Size              = real_size     ;
        m_AllocationIndex   = index         ;
        m_RequestSize       = request_size  ;
        m_Status            = status        ;
        m_Offset            = offset        ;
        if (status != RequestStatus::GOOD){
            m_AllocationIndex = 0;
            m_Data = nullptr;
            m_Size = 0;
            m_Offset = 0;
        }
    }
            
    

    void*           AllocationRequest::GetPointer(void)       const {
        return m_Data; 
    }
    u32             AllocationRequest::GetIndex(void)         const { return m_AllocationIndex; }
    u32             AllocationRequest::GetRequestSize(void)   const { return m_RequestSize; }
    u32             AllocationRequest::GetActualSize(void)    const { return m_Size; }
    RequestStatus   AllocationRequest::GetStatus(void)        const { return m_Status; }
    u32             AllocationRequest::GetOffset(void)        const { return m_Offset; }

}