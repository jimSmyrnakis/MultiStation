#include "Defs.hpp"
#include <iostream>
#include <Core.hpp>

void GLClearErrors(void){
    while(glGetError() != GL_NO_ERROR) ;
}

bool GLLogCall(const char* FunctionName , const char* SourceFile , unsigned int line){
    GLenum error;
    bool nfound = true;
    while( (error = glGetError()) != GL_NO_ERROR ){
        std::cerr << "Error Code ( " << (int)error << 
        " ) at function \" " << FunctionName << 
        " \" , Source File \" " << SourceFile << 
        " \" at line " << line << std::endl; 
        nfound = false;
    }
    return nfound;
}