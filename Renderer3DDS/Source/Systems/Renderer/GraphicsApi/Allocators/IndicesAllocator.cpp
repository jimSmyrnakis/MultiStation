#include "IndicesAllocator.hpp"
#include <glad/gl.h>
#include <memory.h>

#define BLOCK_ALLOCATED         ((u16)0x0001)
#define BLOCK_NEXT              ((u16)0x0002)
#define BLOCK_FIRST             ((u16)0x0004)

#define BLOCK_HAS_ALLOCATED(val)    ((BLOCK_ALLOCATED & (val)) == BLOCK_ALLOCATED)
#define BLOCK_HAS_NEXT(val)         ((BLOCK_NEXT & (val)) == BLOCK_NEXT)
#define BLOCK_IS_FIRST(val)         ((BLOCK_FIRST & (val)) == BLOCK_FIRST)

namespace Game{

    IndicesAllocator::IndicesAllocator (alloc allocation_function  , dalloc free_function ){
        
        if (allocation_function == nullptr){
            ASSERT(0 , "Allocation function is nullptr");
            allocation_function = malloc;
        }
        
        if (free_function == nullptr){
            ASSERT(0 , "Free function is nullptr");
            free_function = free;
        }

        m_Malloc = allocation_function;
        m_Free   = free_function;
        m_AllocationArray = nullptr;
        m_BlockCount = 0;
        m_BlockSize = 0;
        m_HasInitiallized = false;
        m_Size = 0;
        m_ObjectId = 0;
    
    }

    IndicesAllocator::~IndicesAllocator(void){
        if (m_AllocationArray)
            m_Free(m_AllocationArray);
        
    }

    AllocationStatus IndicesAllocator::InitRequest(const u64& size  , const u32& block_size){
        
        if (size == 0){
            ASSERT(0 , "Error -- Size of the allocation is zero , that not possible !!!");
            return AllocationStatus::ERROR_SIZE_IS_ZERO;
        }

        if (block_size < 16){
            ASSERT(0 , "Error -- Block Size is less than 16 , this is not allowed !!!");
            return AllocationStatus::ERROR_BLOCK_LESS_THAN_16;
        }

        if (m_HasInitiallized == true){
            ASSERT(0 , "Warning -- Allocator has initiallized before !!!");
            return AllocationStatus::WARNING_HAS_INITIALLIZED;
        }

        AllocationStatus status = AllocationStatus::INFO_GOOD;

        //If Block is bigger than size
        if (block_size > size){
            status = AllocationStatus::WARNING_SIZE_LESS_THAN_BLOCK;
            m_Size = block_size;
            m_BlockSize = block_size;
            m_BlockCount = 1 ;
        }
        else{
            u32 modulo = size % block_size;
            u32 extra_bytes = block_size - modulo;
            m_Size = size + extra_bytes;
            m_BlockSize = block_size;
            m_BlockCount = m_Size / m_BlockSize;
            
            status = (modulo) ? (AllocationStatus::WARNING_SIZE_NOT_MULTIPLE_OF_BLOCK) : status;
        }
        
        

        //Before initiallization take previus binded buffer 
        i32 old;
        GLCALL( glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER , GL_ELEMENT_ARRAY_BUFFER_BINDING , &old) );

        //now create a new one
        GLCALL( glGenBuffers(1 , &m_ObjectId) );
        //bind it
        GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , m_ObjectId) );
        // Clear any previus error
        GLClearErrors();
        //create the space as invalid yet 
        GLCALL( glBufferData(GL_ELEMENT_ARRAY_BUFFER , m_Size , nullptr , GL_STATIC_DRAW) );
        // the space will start filled data (or at least that is the intent ) before any draw command
        // check if error's exist's 
        if (glGetError() != GL_NO_ERROR){ // if error exist then behave as no memory was the error
            m_Size = 0;
            m_BlockSize = 0;
            m_BlockCount = 0;
            GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , old) );
            return AllocationStatus::ERROR_NO_ENOUGH_MEMORY;
        }

        //If code comes to here then we have a suuccessfull request served from opengl , lets allocate our allocator array
        m_AllocationArray = (u16*)m_Malloc(m_BlockCount * sizeof(u16));
        // check if that was successfull
        if (m_AllocationArray == nullptr){
            m_Size = 0;
            m_BlockSize = 0;
            m_BlockCount = 0;
            // say to opengl that is free to free that memory and do what it wants , i call both invalidate and delete just to be sure 
            // that opengl will take the message
            GLCALL( glInvalidateBufferData(m_ObjectId) );
            GLCALL( glDeleteBuffers(1 , &m_ObjectId) );
            //bind the old buffer in this target
            GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , old) );
            return AllocationStatus::ERROR_NO_ENOUGH_MEMORY;
        }

        //now we know that everything is working , let's initiallize all the array
        memset(m_AllocationArray , 0 , m_BlockCount );
        //set that the allocator has initiallized
        m_HasInitiallized = true;

        //Before Leaving set Back The Previus Buffer for element Arrays
        GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , old) );
        return status;
    }

    AllocationRequest*  IndicesAllocator::RequestSpace(const u32 size){
        if (size == 0){
            ASSERT(0 , "Size is zero !!!");
            return nullptr;
        }

        if (size > m_Size){
            ASSERT(0 , "Requested more space that the total space that is supported !!!");
            return nullptr;
        }

        if (m_HasInitiallized == false){
            ASSERT(0 , " Allocator has not yet initiallized !!!");
            return nullptr;
        }
        
        void* mappedMem = nullptr;
        u32   index = 0;
        u32   modulo = size % m_BlockSize;
        u32   request_blocks_count = (size / m_BlockCount ) + ((modulo) ? 1 : 0);
        RequestStatus status = RequestStatus::GOOD;

        //know we need to find that many contigues blocks in opengl memory 
        bool found = false;

        for (int i =0; i < ( m_BlockCount - request_blocks_count) ; i++){
            
            found = true;
            
            for (int j = i ; j <= (i + request_blocks_count)  ; j++ ){
                if (BLOCK_HAS_ALLOCATED(m_AllocationArray[j]) ){
                    found = false;
                    break;
                }
            }

            if (found)
            {
                index = i;
                break;
            }

        }

        if (!found){ // if not found then give up and create a request status that send that info
            status = RequestStatus::OUT_OF_MEMORY;
            return new AllocationRequest(0 , size , request_blocks_count * m_BlockSize , mappedMem , status , 0 );
        }

        // caculate the offset and length for the request (Mapp)
        u32 mapp_offset = m_BlockSize * index;
        u32 mapp_length = request_blocks_count * m_BlockSize;

        // Bind first the buffer in case is not 
        GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , m_ObjectId) );
        
        // now ask the mapp and set that the contents are not for reading but invalid data that will be validated after
        GLCALL( mappedMem = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER , mapp_offset , mapp_length , GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT ) );
        if (mappedMem == nullptr){
            // Ohh Shit !!!
            ASSERT(0 , "Mapped Failed - the cause could be everything :(");
            status = RequestStatus::NOT_MAPPED;
            return new AllocationRequest(index , size , request_blocks_count * m_BlockSize , mappedMem , status , 0);
        }

        // if everything goes well then allocate this memory
        for (int i = index; i< (index + request_blocks_count ); i++){
            u16 flags = BLOCK_ALLOCATED;
            // first one must set as first
            if (i == index)
                flags |= BLOCK_FIRST;
            //from first one to one before the last one , they must have set the next flag
            if ((i - 1) < (index + request_blocks_count ) )
                flags |= BLOCK_NEXT;

            m_AllocationArray[i] = flags;
        }
        
        //now this pointer is a gate to write the indices that you wish for

        return new AllocationRequest(index , size , request_blocks_count * m_BlockSize , mappedMem , status , mapp_offset );
    }

    void IndicesAllocator::FreeSpace(AllocationRequest* ptr){
        if (m_HasInitiallized == false){
            ASSERT(0 , "Unittilaze allocator" );
            return ;
        }

        if (ptr == nullptr){
            ASSERT(0 , " Warning -- nullptr is not a valid parameter !!!");
            return;
        }
        // check if the pointer is valid 
        if (ptr->GetStatus() != RequestStatus::GOOD){
            delete ptr;
            return;
        }

        //check if the poiter is nullptr
        if (ptr->GetPointer() == nullptr){
            ASSERT(0 , " Warning -- the data Pointer is nullptr !!!");
            delete ptr;
            return;
        }
        //TODO -- check if the pointer says the truth
        //TODO
        //TODO

        // free these blocks from the memory manager
        u32 count_blocks = ptr->GetActualSize() / m_BlockSize;
        for (int i = ptr->GetIndex(); i < ( ptr->GetIndex() + count_blocks ) ; i++ ){
            
        }

        //take the old buffer 
        i32 old = 0;
        GLCALL( glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER , GL_ELEMENT_ARRAY_BUFFER_BINDING , &old) );

        // bind the buffer
        GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , m_ObjectId) );
        // invalidate that space of the buffer (opengl may use it for other things )
        GLCALL( glInvalidateBufferSubData(GL_ELEMENT_ARRAY_BUFFER , ptr->GetOffset() , ptr->GetActualSize()) );

        //bind the previus one
        GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , old) );
        delete ptr;
    }

    void IndicesAllocator::Update( const AllocationRequest& request ){
        if (request.GetStatus() != RequestStatus::GOOD){
            ASSERT(0 , "Request is at bad status (not valid) !!!");
            return;
        }


    }

}