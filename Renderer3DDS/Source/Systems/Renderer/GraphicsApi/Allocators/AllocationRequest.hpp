#pragma once 

#include <Defs.hpp>

namespace Game{

    enum class RequestStatus{
        OUT_OF_MEMORY   ,  
        NOT_MAPPED      ,   // May the more scary one as it means that opengl failed to map memory (and even scarier if is not because of our virtual memory space ) 
        GOOD            
    };

    class AllocationRequest{
        public:
            AllocationRequest(u32 index , u32 request_size , u32 real_size , void* mapped_ptr , RequestStatus status , u32 offset);
            
            ~AllocationRequest(void) = default;

            void*           GetPointer(void)        const;
            u32             GetIndex(void)          const;
            u32             GetRequestSize(void)    const;
            u32             GetActualSize(void)     const;
            RequestStatus   GetStatus(void)         const;
            u32             GetOffset(void)         const;

            AllocationRequest(const AllocationRequest& copy)            = delete;
            AllocationRequest& operator=(const AllocationRequest& copy) = delete;
            AllocationRequest(AllocationRequest&& move)                 = delete;
            AllocationRequest& operator= (AllocationRequest&& move)     = delete;

        private:
            u32             m_AllocationIndex;
            u32             m_RequestSize;
            u32             m_Size;
            void*           m_Data;
            RequestStatus   m_Status;
            u32             m_Offset;
    };
}