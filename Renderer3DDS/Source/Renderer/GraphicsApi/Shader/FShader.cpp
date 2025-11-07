#include "FShader.hpp"
#include <GL/glew.h>

namespace MultiStation {
    FShader::FShader(const char* source){
        //Create Shader
        GLCALL( m_Id = glCreateShader(GL_FRAGMENT_SHADER) );
        //Set Shader Source
        GLCALL( glShaderSource(m_Id , 1 , &source , nullptr) );
        //Compile Shader
        GLCALL( glCompileShader(m_Id) );
        int ShaderCompiled;
        //Get Compilation Status
        GLCALL( glGetShaderiv(m_Id, GL_COMPILE_STATUS, &ShaderCompiled) );
        /*Check Compilation Status*/
        if (ShaderCompiled == GL_FALSE){
            // if compilation failed
            char message[1024];
            int32_t  len = 1024;
            GLCALL( glGetShaderInfoLog(m_Id, 1024, &len, message) );
            ASSERT(0 , (const char*)message);
            m_HasCompile = false;
             
        }
        else m_HasCompile = true;

    }

    FShader::~FShader(void){
        glDeleteShader(m_Id);
    }

    uint32_t FShader::GetId(void) const { return m_Id; }

    bool FShader::HasCompiled(void) const { return m_HasCompile; }
}