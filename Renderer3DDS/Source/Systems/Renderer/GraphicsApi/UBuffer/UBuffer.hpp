#pragma once

#include "../../../../Defs.hpp"
#include <stdio.h>
#include <stdlib.h>
#include "../Format/Format.hpp"

namespace Game{


    typedef ShaderDataType UniformType;

    #define GetUniformTypeFromOpenGLUniformType OpenGLDataTypeToShaderDataType
    #define GetUniformTypeSize                  SizeOfShaderDataType
    #define GetOpenGLUniformTypeFromUniformType ShaderDataTypeToOpenGLDataType

    class UBuffer{

        public:

            UBuffer(
                u32     count                       , 
                u32     size                        ,  
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
                u32 GetUniformsCount(void) const;
                /*return the total uniforms this ubuffer can have*/
                u32 GetDefinedUniformsCount(void) const;
                /*return the total uniforms this ubuffer have take*/
                u32 GetUndefinedUniformsCount(void) const;
                /*return the total uniforms this ubuffer can take*/
                void GetUniformNameByIndex(const u32& index , char* name , u32 maxLength );
                /**/
                bool AddUniform(UniformType type , char* name , u32 length , void* data );
                
             
                void*       GetUniformPointerByName(const char* name) const;
                UniformType GetUniformTypeByName(const char* name) const;

                // TODO - make GetUniformIdByName public and make a method GetUniformNameById 
                const char* GetUniformNameById(const u32& id) const;
                u32 GetUniformIdByName(const char* name) const;
        private:
            mutable void*       m_UniformsMemory;   // The contigues memory with all uniform variables
            // plus is mutable so a const class type of this still can change the internal data but not add
            // or remove uniforms only change their contents
            mutable void*       m_MemCurrent;
            // defines the first free byte address after each time we add a uniform
            u32                 m_Size;             // The size of thos memory in bytes
            u32                 m_CountUniforms;    // The total number of uniforms 
            u32                 m_SpecifiedUniforms;// The number of uniforms that have already added
            UniformType*        m_UniformsTypes;    // Each uniform type (a type defines the total number of elements and the size of element )
            u64*                m_UniformsPointers; // For Each Uniform where is the first byte of it
            char**              m_UniformsNames;    // For each Uniform the name in the shader
            u32*                m_NamesLength;      // The total number of characters for each name (0 means null terminated string)
            alloc               m_Malloc;           // a function pointer to a given malloc like function
            dalloc              m_Free;             // a function pointer to a given free like function
            bool                m_HasMoved;         // becames true only when this instance moved its data to other instance
    };
}