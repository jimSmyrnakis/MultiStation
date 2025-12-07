#include "UBuffer.hpp"
#include "../Init.hpp"

/*==================================================================================
/*==================================================================================*/
/*==================================================================================*/
namespace MultiStation{
    UBuffer::UBuffer(
              ::uint32_t  count                , 
              ::uint32_t  size                 ,  
        struct Allocator allocator        )
    {

        //check parameters
        ASSERT(size , "The Size of the uniform buffer is defined zero !");
        ASSERT(count, "The number of uniforms can not be zero !");
        ASSERT(allocator.memalloc, "No Allocation function is given !!!");
        ASSERT(allocator.memfree, "No Free Function is Given");
        
        //Even if parameters are bad still define a default behavor even if is not right
        if (allocator.memalloc == nullptr)
            allocator.memalloc = malloc;
        if (allocator.memfree == nullptr)
            allocator.memfree = free;
        if (size == 0 ) 
            size = 128;   
        if (count == 0) 
            count= 24;
        
        //Initiallize all variables and allocate the space needed for each
        m_Allocator         = allocator;
        m_NamesLength       = nullptr;
        m_UniformsNames     = nullptr;
        m_UniformsPointers  = nullptr;
        m_UniformsTypes     = nullptr;
        m_SpecifiedUniforms = 0;
        m_CountUniforms     = count;
        m_Size              = size;
        m_UniformsMemory    = nullptr;
        m_HasMoved          = false;
        m_MemCurrent        = nullptr;
        
        // allocate the total size for the uniforms data from cpu side
        m_UniformsMemory = m_Allocator.memalloc(size);
        if (m_UniformsMemory == nullptr){
            ASSERT(0 , "No memory !");
            m_CountUniforms     = 0;
            m_Size              = 0;
            return;
        }
        
        
        
        // allocate count uniform type definitions
        m_UniformsTypes = (UniformType*)m_Allocator.memalloc(m_CountUniforms*sizeof(UniformType));
        if (m_UniformsTypes == nullptr){
            ASSERT(0 , "No memory !");
            m_Allocator.memfree(m_UniformsMemory);
            m_UniformsMemory = nullptr;
            m_CountUniforms     = 0;
            m_Size              = 0;
            return ;
        }

        // allocate count uniform offset's for define with each uniform add method 
        m_UniformsPointers = (uintptr_t*)m_Allocator.memalloc(m_CountUniforms*sizeof(uintptr_t));
        if (m_UniformsPointers == nullptr){
            ASSERT(0 , "No memory !");
            m_Allocator.memfree(m_UniformsMemory);
            m_Allocator.memfree(m_UniformsTypes);
            m_UniformsTypes = nullptr;
            m_UniformsMemory = nullptr;
            m_CountUniforms     = 0;
            m_Size              = 0;
            return ;
        }

        // allocate count string pointers for each uniform name
        m_UniformsNames = (char**)m_Allocator.memalloc(m_CountUniforms*sizeof(char*));
        if (m_UniformsNames == nullptr){
            ASSERT(0 , "No memory !");
            m_Allocator.memfree(m_UniformsMemory);
            m_Allocator.memfree(m_UniformsTypes);
            m_Allocator.memfree(m_UniformsPointers);
            m_UniformsPointers = nullptr;
            m_UniformsTypes = nullptr;
            m_UniformsMemory = nullptr;
            m_CountUniforms     = 0;
            m_Size              = 0;
            return ;
        }

        // allocate count ::uint32_t number's for each uniform name length
        m_NamesLength = (::uint32_t*)m_Allocator.memalloc(m_CountUniforms*sizeof(::uint32_t));
        if (m_NamesLength == nullptr){
            ASSERT(0 , "No memory !");
            m_Allocator.memfree(m_UniformsMemory);
            m_Allocator.memfree(m_UniformsTypes);
            m_Allocator.memfree(m_UniformsPointers);
            m_Allocator.memfree(m_UniformsNames);
            m_UniformsNames = nullptr;
            m_UniformsPointers = nullptr;
            m_UniformsTypes = nullptr;
            m_UniformsMemory = nullptr;
            m_CountUniforms     = 0;
            m_Size              = 0;
            return ;
        }

        //point the first available byte
        m_MemCurrent = m_UniformsMemory;

        
    }

    UBuffer::UBuffer(UBuffer&& move) noexcept{
        m_Allocator         = move.m_Allocator;
        m_NamesLength       = move.m_NamesLength;
        m_UniformsNames     = move.m_UniformsNames;
        m_UniformsPointers  = move.m_UniformsPointers;
        m_UniformsTypes     = move.m_UniformsTypes;
        m_SpecifiedUniforms = move.m_SpecifiedUniforms;
        m_CountUniforms     = move.m_CountUniforms;
        m_Size              = move.m_Size;
        m_UniformsMemory    = move.m_UniformsMemory;
        m_HasMoved          = false;
        m_MemCurrent        = move.m_MemCurrent;

        move.m_Allocator.memalloc   = nullptr   ;
        move.m_Allocator.memfree    = nullptr   ;
        move.m_NamesLength          = nullptr   ;
        move.m_UniformsNames        = nullptr   ;     
        move.m_UniformsPointers     = nullptr   ;
        move.m_UniformsTypes        = nullptr   ;
        move.m_SpecifiedUniforms    = 0         ;
        move.m_CountUniforms        = 0         ;
        move.m_Size                 = 0         ;
        move.m_UniformsMemory       = nullptr   ;
        move.m_HasMoved             = true      ;
        move.m_MemCurrent           = nullptr   ;
    }

    UBuffer& UBuffer::operator=(UBuffer&& move) noexcept{
        m_Allocator         = move.m_Allocator;
        m_NamesLength       = move.m_NamesLength;
        m_UniformsNames     = move.m_UniformsNames;
        m_UniformsPointers  = move.m_UniformsPointers;
        m_UniformsTypes     = move.m_UniformsTypes;
        m_SpecifiedUniforms = move.m_SpecifiedUniforms;
        m_CountUniforms     = move.m_CountUniforms;
        m_Size              = move.m_Size;
        m_UniformsMemory    = move.m_UniformsMemory;
        m_HasMoved          = false;

        move.m_Allocator.memalloc   = nullptr  ;
        move.m_Allocator.memfree    = nullptr  ;
        move.m_NamesLength          = nullptr  ;
        move.m_UniformsNames        = nullptr  ;
        move.m_UniformsPointers     = nullptr  ;
        move.m_UniformsTypes        = nullptr  ;
        move.m_SpecifiedUniforms    = 0        ;
        move.m_CountUniforms        = 0        ;
        move.m_Size                 = 0        ;
        move.m_UniformsMemory       = nullptr  ;
        move.m_HasMoved             = true     ;

        return *this;
    }

    UBuffer::~UBuffer(void){
        if (m_HasMoved)
            return;
        //free the memory of the uniform data
        if (m_UniformsMemory)
            m_Allocator.memfree(m_UniformsMemory);
        //free the memory of the names length for each uniform
        if (m_NamesLength)
            m_Allocator.memfree(m_NamesLength);
        //free the memory from all names of each uniform and the double ptr
        if (m_UniformsNames){
            for (int i  = 0 ; i < m_CountUniforms; i++)
                if  (m_UniformsNames[i])
                    m_Allocator.memfree(m_UniformsNames[i]);
            m_Allocator.memfree(m_UniformsNames);
        }
        //free uniforms offset
        if (m_UniformsPointers)
            m_Allocator.memfree(m_UniformsPointers);
        //free uniforms types
        if (m_UniformsTypes)
            m_Allocator.memfree(m_UniformsTypes);
        // all freed
    }

}



/*==================================================================================
/*==================================================================================*/
/*==================================================================================*/
/*==================================================================================
/*==================================================================================*/
/*==================================================================================*/
/*==================================================================================
/*==================================================================================*/
/*==================================================================================*/
/*==================================================================================
/*==================================================================================*/
/*==================================================================================*/




namespace MultiStation{
    
    ::uint32_t UBuffer::GetUniformsCount(void) const{
        return m_CountUniforms;
    }
    /*return the total uniforms this ubuffer can have*/
    ::uint32_t UBuffer::GetDefinedUniformsCount(void) const{
        return m_SpecifiedUniforms;
    }
    /*return the total uniforms this ubuffer have take*/
    ::uint32_t UBuffer::GetUndefinedUniformsCount(void) const{
        return m_CountUniforms - m_SpecifiedUniforms;
    }
    void UBuffer::GetUniformNameByIndex(const ::uint32_t& index , char* name , ::uint32_t maxLength ){
        if (m_HasMoved){
            ASSERT(0 , "This Uniforms Buffer has move its contents to other ubuffer !!!");
            return;
        }

        if (index >= m_SpecifiedUniforms){
            ASSERT(0 , "Out Of range !!!");
            return;
        }

        if (maxLength == 0){
            ASSERT(0 , "Minimum length must be 1 ");
            return ;
        }

        strncpy_s(name , maxLength , m_UniformsNames[index] , maxLength);

    }
    /*return the total uniforms this ubuffer can take*/
    bool UBuffer::AddUniform(UniformType type , char* name , ::uint32_t length , void* data ){

        //check if is a trash instance
        if (m_HasMoved){
            ASSERT(0 , "This Uniforms Buffer has move its contents to other ubuffer !!!");
            return false;
        }

        // name required
        if (name == nullptr){
            ASSERT(0 , "Can't add a uniform without a name !!!");
            return false;
        }

        // do we have at least one more position free to add one more (check 1)
        if (m_SpecifiedUniforms >= m_CountUniforms){
            ASSERT(0 , "Uniform's Buffer is full !!!");
            return false;
        }

        // find a allign memory address inside our buffer for our uniform data (align by its data size) 
        void* alligmentPos        = m_MemCurrent;
        uintptr_t   TypeSize      = GetUniformTypeSize(type);
        uintptr_t   Modulo        = ((uintptr_t)alligmentPos) % TypeSize;
        uintptr_t   ExtraCount    = (Modulo) ? (TypeSize - Modulo) : 0;
        alligmentPos        = (void*) ( (uintptr_t)m_MemCurrent + ExtraCount);
        // is there any free space inside our buffer for the uniform ? 
        if (((uintptr_t)alligmentPos + TypeSize) >= ((uintptr_t)m_UniformsMemory + m_Size)){
            ASSERT(0 , "No Enough Uniform Data Space to add a new uniform !!!");
            return false;
        }
        m_MemCurrent = (void*)((uintptr_t)alligmentPos + TypeSize);
        if (data != nullptr) // if there are some data given , copy them to the new space 
            memcpy(alligmentPos , data , TypeSize);
        
        // add one to the actual number of uniforms 
        m_SpecifiedUniforms++;

        // add the pointer to the uniform address
        m_UniformsPointers[m_SpecifiedUniforms - 1] = (uintptr_t)alligmentPos;

        // how big is the name of it ?
        m_NamesLength[m_SpecifiedUniforms - 1] =(length) ? (length+1) : (strlen(name) + 1);
        ::uint32_t maxlen = m_NamesLength[m_SpecifiedUniforms - 1];
        
        // allocate space for the uniform name
        m_UniformsNames[m_SpecifiedUniforms - 1] 
        = (char*)m_Allocator.memalloc(maxlen );
        
        
        // copy to that space
        strncpy_s(
            m_UniformsNames[m_SpecifiedUniforms - 1] , maxlen  , name ,
            maxlen  );
        // set null char
        char* Uname = m_UniformsNames[m_SpecifiedUniforms - 1];
        Uname[maxlen - 1] = '\0'; // null terminated character
        
        // set his type
        m_UniformsTypes[m_SpecifiedUniforms - 1] = type;
        
        // return all good
        return true;
    }


    ::uint32_t UBuffer::GetUniformIdByName(const char* name) const{
        if (m_HasMoved){
            ASSERT(0 , "This Uniforms Buffer has move its contents to other ubuffer !!!");
            return 0xFFFFFFFF;
        }

        if (m_SpecifiedUniforms == 0){
            ASSERT(0 , "No Uniforms have added yet !!!");
            return 0xFFFFFFFF;
        }

        if (m_UniformsMemory == nullptr){
            ASSERT(0 , "No Memory !!!");
            return 0xFFFFFFFF;
        }

        ::uint32_t id = 0xFFFFFFFF;
        for(::uint32_t i = 0; i < m_SpecifiedUniforms; i++){
            
            if (strcmp(m_UniformsNames[i] , name) == 0){
                id = i;
                break;
                
            }
            
        }

        return id;
    }
    void* UBuffer::GetUniformPointerByName(const char* name) const{
        ::uint32_t id = GetUniformIdByName(name);

        if (id == 0xFFFFFFFF){
            ASSERT(0 , "Uniform Can't found !!!");
            return nullptr;
        }
        
        return (void*)m_UniformsPointers[id]; 
    }

    UniformType UBuffer::GetUniformTypeByName(const char* name) const{
        ::uint32_t id = GetUniformIdByName(name);

        if (id == 0xFFFFFFFF){
            ASSERT(0 , "Uniform Can't found !!!");
            return (UniformType)0;
        }

        return m_UniformsTypes[id];
    }

    const char* UBuffer::GetUniformNameById(const ::uint32_t& id) const{
        if (m_HasMoved){
            ASSERT(0 , "This Uniforms Buffer has move its contents to other ubuffer !!!");
            return nullptr;
        }

        if (m_SpecifiedUniforms == 0){
            ASSERT(0 , "No Uniforms have added yet !!!");
            return nullptr;
        }

        if (m_UniformsMemory == nullptr){
            ASSERT(0 , "No Memory !!!");
            return nullptr;
        }

        if (id >= m_SpecifiedUniforms){
            ASSERT(0 , "OUT OF RANGE !!!");
            return nullptr;
        }

        return m_UniformsNames[id];
    }

    void UBuffer::RedirectUniformPointerByName(const char* name, void* newPointer) const {
		::uint32_t id = GetUniformIdByName(name);
		if (id == 0xFFFFFFFF) {
			ASSERT(0, "Uniform Can't found !!!");
			return;
		}
		if (m_UniformsPointers == nullptr) {
			ASSERT(0, "No Memory !!!");
			return;
		}

		if (newPointer == nullptr) {
			ASSERT(0, "Can't redirect to nullptr !!!");
			return;
		}

        
		m_UniformsPointers[id] = (uintptr_t)newPointer;
    }
                
}





