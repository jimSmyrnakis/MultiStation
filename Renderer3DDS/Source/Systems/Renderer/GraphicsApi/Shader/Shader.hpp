#pragma once 
#include <Defs.hpp>
#include <glm/glm.hpp>
#include "../UBuffer/UBuffer.hpp"
#include "VShader.hpp"
#include "FShader.hpp"
#include <stdlib.h>

namespace Game{


    class Shader{
        

        public:
            Shader(VShader& Vertex , FShader& Fragment , alloc Malloc = malloc, dalloc Free = free);
            ~Shader(void);

            void Bind(void) ;
            void Unbind(void) ;

            UBuffer* GetUniforms(void) ;

            i32 MaxUniformNameLength(void) const;


        private:
            
            void FindUniforms(void);
            void FindAttributes(void);
            void UpdateUniforms(void);
            
        private:
            u32             m_ProgramId         ;   // id of the program
            VShader*        m_Vertex            ;   // Vertex Shader
            FShader*        m_Fragment          ;   // Fragment Shader
            bool            m_HasProgram        ;   // Used to define if a program has created (usefull for CreateProgram method)  

            UBuffer*        m_Uniforms          ;   // a uniform's buffer for this program 

            u32             m_AttributesCount   ;   // Number of input attributes to the vertex shader -- TODO
            char**          m_AttributeNames    ;   // The Names of the attributes -- TODO
            u32*            m_AttributesLoc     ;   // The Location of each attribute -- TODO
            

            alloc           m_Malloc            ;   // memory allocation function
            dalloc          m_Free              ;   // memory free function

    };

}