#pragma once 

#include "AllocationRequest.hpp"
#include <Defs.hpp>
#include <stdlib.h>

namespace Game{
    enum class AllocationStatus{
        INFO_GOOD                           ,   // Allocation is successfull 

        ERROR_NO_ENOUGH_MEMORY              ,   // Allocation failed do to no enough space 
        ERROR_BLOCK_LESS_THAN_16            ,   // Allocation failed because the size of the block is less than 16 bytes
        ERROR_SIZE_IS_ZERO                  ,   // Allocation failed because the size is zero 

        WARNING_SIZE_LESS_THAN_BLOCK        ,   // The Size of the allocation is less that what the block size is
        WARNING_SIZE_NOT_MULTIPLE_OF_BLOCK  ,   // The Size of the allocation is not multiple of the block (this is just a warning )
        WARNING_HAS_INITIALLIZED                // Recall to the initiallization method that previusly was successfull will do nothing and return this code
    };

    class IndicesAllocator{
        public:


        public:
            IndicesAllocator (alloc allocation_function = malloc , dalloc free_function = free);
            ~IndicesAllocator(void);

            AllocationStatus    InitRequest(const u64& size  , const u32& block_size);
            AllocationRequest*  RequestSpace(const u32 size);
            void                Update( const AllocationRequest& request );
            void                FreeSpace(AllocationRequest* ptr);
            
        private:
            u64         m_Size;             // The total size in bytes of the element array buffer 
            u32         m_BlockSize;        // The Size of the block in bytes that the space will broke in these peaces
            alloc       m_Malloc;           // The malloc that this class will use (this definition is temporary)
            dalloc      m_Free;             // The free that this class will use (this definition is temporary) 
            bool        m_HasInitiallized;  // Is true if the class has initiallize space 

            u16*        m_AllocationArray;  // defines what blocks have allocated and where the first and last allocated blocks per allocation are
            u32         m_BlockCount;       // The number of blocks that have allocated
            // Basicly the blocks number multiple by the block size is the final actual size of the element array size 

            //specifacly for ELEMENT_ARRAY 
            u32         m_ObjectId;         // The buffer object id 
            
            
            
    };
}