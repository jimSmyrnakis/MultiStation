#pragma once 
#include <Defs.hpp>
#include <glm/glm.hpp>
#include "../UBuffer/UBuffer.hpp"
#include "VShader.hpp"
#include "FShader.hpp"
#include <stdlib.h>

namespace MultiStation{


    class Shader{
        

        public:
            Shader(VShader& Vertex , FShader& Fragment , alloc Malloc = malloc, dalloc Free = free);
            ~Shader(void);

            void Bind(void) ;
            void Unbind(void) ;

            UBuffer* GetUniforms(void) ;

            int32_t MaxUniformNameLength(void) const;


        private:
            
            void FindUniforms(void);
            void FindAttributes(void);
            void UpdateUniforms(void);
            
        private:
            uint32_t        m_ProgramId         ;   // id of the program
            VShader*        m_Vertex            ;   // Vertex Shader
            FShader*        m_Fragment          ;   // Fragment Shader
            bool            m_HasProgram        ;   // Used to define if a program has created (usefull for CreateProgram method)  

            UBuffer*        m_Uniforms          ;   // a uniform's buffer for this program 

            uint32_t        m_AttributesCount   ;   // Number of input attributes to the vertex shader -- TODO
            char**          m_AttributeNames    ;   // The Names of the attributes -- TODO
            uint32_t*       m_AttributesLoc     ;   // The Location of each attribute -- TODO
            

            alloc           m_Malloc            ;   // memory allocation function
            dalloc          m_Free              ;   // memory free function

    };

}