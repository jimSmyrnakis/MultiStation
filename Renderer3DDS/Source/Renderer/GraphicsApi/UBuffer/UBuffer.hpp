#pragma once

#include<Defs.hpp>
#include <Renderer/GraphicsApi/Format/Format.hpp>

namespace MultiStation{


    typedef ShaderDataType UniformType;

    #define GetUniformTypeFromOpenGLUniformType OpenGLDataTypeToShaderDataType
    #define GetUniformTypeSize                  SizeOfShaderDataType
    #define GetOpenGLUniformTypeFromUniformType ShaderDataTypeToOpenGLDataType

    class UBuffer{

        public:

            UBuffer(
                uint32_t     count                       , 
                uint32_t     size                        ,  
                alloc   allocateFunction = malloc   , 
                dalloc  dallocateFunction = free    );

            UBuffer(const UBuffer& copy) = delete; // for security reasons
            UBuffer(UBuffer&& move);
            //In Constructor the total number of uniforms must defined in general and the total size of memory too
            //the reason to define the size of memory is more for perfomance as uniforms are not most of the times
            //more than 10 but who ever give the size must be sure that all uniforms will fit in with alligment's
            //otherwise an debug break will generated . 

            ~UBuffer(void);
            /*Constructor is responsible for allocating all the nessesary space where destructor is responsible for d-allocating
              all this space */
        
            UBuffer& operator=(const UBuffer& copy) = delete; // for security issues
            UBuffer& operator=(UBuffer&& move);

        public:
                uint32_t GetUniformsCount(void) const;
                /*return the total uniforms this ubuffer can have*/
                uint32_t GetDefinedUniformsCount(void) const;
                /*return the total uniforms this ubuffer have take*/
                uint32_t GetUndefinedUniformsCount(void) const;
                /*return the total uniforms this ubuffer can take*/
                void GetUniformNameByIndex(const uint32_t& index , char* name , uint32_t maxLength );
                /**/
                bool AddUniform(UniformType type , char* name , uint32_t length , void* data );
                
             
                void*       GetUniformPointerByName(const char* name) const;
                UniformType GetUniformTypeByName(const char* name) const;

                // TODO - make GetUniformIdByName public and make a method GetUniformNameById 
                const char* GetUniformNameById(const uint32_t& id) const;
                uint32_t GetUniformIdByName(const char* name) const;
        private:
            mutable void*       m_UniformsMemory;   // The contigues memory with all uniform variables
            // plus is mutable so a const class type of this still can change the internal data but not add
            // or remove uniforms only change their contents
            mutable void*       m_MemCurrent;
            // defines the first free byte address after each time we add a uniform
            uint32_t            m_Size;             // The size of thos memory in bytes
            uint32_t            m_CountUniforms;    // The total number of uniforms 
            uint32_t            m_SpecifiedUniforms;// The number of uniforms that have already added
            UniformType*        m_UniformsTypes;    // Each uniform type (a type defines the total number of elements and the size of element )
            uintptr_t*           m_UniformsPointers; // For Each Uniform where is the first byte of it
            char**              m_UniformsNames;    // For each Uniform the name in the shader
            uint32_t*           m_NamesLength;      // The total number of characters for each name (0 means null terminated string)
            alloc               m_Malloc;           // a function pointer to a given malloc like function
            dalloc              m_Free;             // a function pointer to a given free like function
            bool                m_HasMoved;         // becames true only when this instance moved its data to other instance
    };
}