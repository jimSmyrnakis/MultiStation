#include "Shader.hpp"
#include "VShader.cpp"
#include "FShader.cpp"
#include <glad/gl.h>

namespace Game{

    

    Shader::Shader(VShader& Vertex , FShader& Fragment , alloc Malloc, dalloc Free ){
        m_Fragment  = nullptr;
        m_Vertex    = nullptr;
        m_Uniforms  = nullptr;

        ASSERT(Malloc , "nullptr for memory allocator function !!!");
        if (Malloc == nullptr)
            Malloc = malloc;
        
        ASSERT(Free , "nullptr for memory free function !!!");
        if (Free == nullptr)
            Free = free;
        
        m_HasProgram = true;

        if (Vertex.HasCompiled() == false){
            ASSERT(0 , "Vertex Shader has not comppiled !!!");
            m_HasProgram = false;
            return;
        }

        if (Fragment.HasCompiled() == false){
            ASSERT(0, "Fragment Shader has not compiled !!!");
            m_HasProgram = false;
            return;
        }

        GLCALL( m_ProgramId = glCreateProgram() );

        if (m_ProgramId == 0){
            m_HasProgram = false;
            return ;
        }

        GLCALL( glAttachShader(m_ProgramId , Vertex.GetId()) );
        GLCALL( glAttachShader(m_ProgramId , Fragment.GetId()) );

        GLCALL( glLinkProgram(m_ProgramId) );
        GLint program_linked;
        glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &program_linked);
        if (program_linked != GL_TRUE)
        {
            GLsizei log_length = 0;
            GLchar message[1024];
            glGetProgramInfoLog(m_ProgramId, 1024, &log_length, message);
            ASSERT(0 , message);
            m_HasProgram = false;
            return;
        }

        m_Malloc    = Malloc;
        m_Free      = Free;
        m_Vertex    = &Vertex;
        m_Fragment  = &Fragment;

        m_Uniforms = nullptr;
        m_AttributeNames = nullptr;
        m_AttributesCount = 0;
        m_AttributesLoc = nullptr;
        

        FindUniforms();
        FindAttributes();

    }

    Shader::~Shader(void){
        

        if (m_Fragment != nullptr){
           GLCALL( glDetachShader(m_ProgramId , m_Fragment->GetId() ) );
        }

        if (m_Vertex != nullptr){
            GLCALL( glDetachShader(m_ProgramId , m_Vertex->GetId()) );
        }

        GLCALL( glDeleteProgram(m_ProgramId));
        if (m_Uniforms)
            delete m_Uniforms;
        
        
    }

    


    void Shader::Bind(void) {
        GLCALL( glUseProgram(m_ProgramId));
        UpdateUniforms();
    }

    void Shader::Unbind(void) {
        GLCALL( glUseProgram(0));
    }
    
    UBuffer* Shader::GetUniforms(void) { 
        ASSERT(m_HasProgram , "No Program is Created !!!");
        ASSERT(m_Uniforms , "Uniforms buffer is nullptr !!!");
        return m_Uniforms; 
    }

    #define CheckAllocation(pointer) if ((pointer) == nullptr) { ASSERT(0 , "No Memory !!!"); return; }

    //TODO - Need's better handling in memory allocation's and free's of them 
    void Shader::FindUniforms(void){
        u32 UniformsCount;
        if (m_HasProgram == false){
            ASSERT(m_HasProgram , "No Program is Created !!!");
            return;
        }
        
        //Get the number of all uniforms
        i32 NumOfUniforms = 0;
        glGetProgramiv(m_ProgramId , GL_ACTIVE_UNIFORMS , &NumOfUniforms);
        if (NumOfUniforms == 0){
            ASSERT(0 , "No Uniforms !!!");
            return;
        }
        
        //Set the variable for the type of each uniform
        i32 *UniformsTypes = (i32*)m_Malloc(NumOfUniforms * sizeof(i32));
        CheckAllocation(UniformsTypes);

        //Set the variable for the lengths of character of each name of the uniforms
        u32* UniformLengths = (u32*)m_Malloc(NumOfUniforms * sizeof(u32));
        CheckAllocation(UniformLengths);

        //set the variable with all the names with size of the maximum uniform name just to be sure
        char** UniformsNames = (char**)m_Malloc(NumOfUniforms * sizeof(char*));
        CheckAllocation(UniformsNames);
        
        //find the maximum name size
        i32 maxName;
        glGetProgramiv(m_ProgramId , GL_ACTIVE_UNIFORM_MAX_LENGTH , &maxName);
        if (maxName ==0){
            ASSERT(0 , "Something Is Wrong !@!!!!");
            return;
        }

        //allocate all names with the maximum name characters size
        for ( int i = 0 ; i < NumOfUniforms; i++){
            UniformsNames[i] = (char*)m_Malloc(maxName + 1);
            
        }

        //set a variable for having all the size's of exh uniform
        i32* UniformsSizes = (i32*)m_Malloc(NumOfUniforms * sizeof(i32));
        CheckAllocation(UniformsSizes );

        //now get all the uniforms details
        u32 TotalSize = 0;
        for (int i = 0; i < NumOfUniforms; i++){
            GLCALL( glGetActiveUniform(
                m_ProgramId , i , maxName + 1 , (GLsizei*)&UniformLengths[i] , 
                &UniformsSizes[i] , (GLenum*)&UniformsTypes[i]  , UniformsNames[i]) );
                TotalSize += GetUniformTypeSize(GetUniformTypeFromOpenGLUniformType(UniformsTypes[i])); 
                
            
        }

        //now create the uniform buffer 
        m_Uniforms = new UBuffer(NumOfUniforms , 2*TotalSize , m_Malloc , m_Free);
        CheckAllocation(m_Uniforms);

        for (int i = 0; i < NumOfUniforms; i++){
            m_Uniforms->AddUniform(
                GetUniformTypeFromOpenGLUniformType(UniformsTypes[i]) ,
                UniformsNames[i] , UniformLengths[i] , nullptr );
        }

        //Free all the memory - Still the code is bad 
        m_Free(UniformLengths);
        m_Free(UniformsTypes);
        m_Free(UniformsSizes);
        for (int i =0 ;i < NumOfUniforms; i++){
            m_Free(UniformsNames[i]);
        }
        m_Free(UniformsNames);
        


    }

    static void SetUniformDataBasedOnType(UniformType type , u32 location , void* data ){
        GLenum Utype = GetOpenGLUniformTypeFromUniformType(type);
        
        switch(type){
            case ShaderDataType::FLOAT      :   glUniform1f(location , *((float*)data) );                       break;
            case ShaderDataType::VEC2F      :   glUniform2fv(location , 1 , ((float*)data) );                   break;
            case ShaderDataType::VEC3F      :   glUniform3fv(location , 1 , ((float*)data) );                   break;
            case ShaderDataType::VEC4F      :   glUniform4fv(location , 1 , ((float*)data) );                   break;
            case ShaderDataType::MAT3F_T    :   glUniformMatrix3fv(location , 1 , GL_TRUE ,  ((float*)data) );  break;
            case ShaderDataType::MAT4F_T    :   glUniformMatrix4fv(location , 1 , GL_TRUE ,  ((float*)data) );  break;

            case ShaderDataType::INT        :   glUniform1i(location , *((i32*)data) );                         break;
            case ShaderDataType::VEC2I      :   glUniform2iv(location , 1 , ((i32*)data) );                     break;
            case ShaderDataType::VEC3I      :   glUniform3iv(location , 1 , ((i32*)data) );                     break;
            case ShaderDataType::VEC4I      :   glUniform4iv(location , 1 , ((i32*)data) );                     break;

            case ShaderDataType::UINT       :   glUniform1ui(location , *((u32*)data) );                        break;
            case ShaderDataType::VEC2UI     :   glUniform2uiv(location , 1 , ((u32*)data) );                    break;
            case ShaderDataType::VEC3UI     :   glUniform3uiv(location , 1 , ((u32*)data) );                    break;
            case ShaderDataType::VEC4UI     :   glUniform4uiv(location , 1 , ((u32*)data) );                    break;

            case ShaderDataType::SAMPLER_2D :   glUniform1i(location , *((i32*)data) );                         break;
            case ShaderDataType::ISAMPLER_2D:   glUniform1i(location , *((i32*)data) );                         break;
            case ShaderDataType::USAMPLER_2D:   glUniform1i(location , *((i32*)data) );                         break;

            default : ASSERT(0 , "Undefined Shader Data Type !!!"); return ;
        }
    }

    void Shader::UpdateUniforms(void){
        if (m_Uniforms == nullptr)
            return;

        int unfs = m_Uniforms->GetDefinedUniformsCount();
        char name[100] = {0};
        for (int i = 0; i < unfs ; i++ ){
            m_Uniforms->GetUniformNameByIndex(i , name , 100);
            int loc = glGetUniformLocation(m_ProgramId , name);
            SetUniformDataBasedOnType(m_Uniforms->GetUniformTypeByName(name) , loc , m_Uniforms->GetUniformPointerByName(name));
        }
    }

    i32 Shader::MaxUniformNameLength(void) const {
        if (m_HasProgram == false){
            ASSERT(m_HasProgram , "No Program is Created !!!");
            return 0;
        }
        i32 maxName = 0;
        glGetProgramiv(m_ProgramId , GL_ACTIVE_UNIFORM_MAX_LENGTH , &maxName);
        return maxName;
    }

    void Shader::FindAttributes(void){
        if (m_HasProgram == false){
            ASSERT(m_HasProgram , "No Program is Created !!!");
            return;
        }

        //Get the number of all attributes
        i32 NumOfAttributes = 0;
        glGetProgramiv(m_ProgramId , GL_ACTIVE_ATTRIBUTES , &NumOfAttributes);
        if (NumOfAttributes == 0){
            ASSERT(0 , "No Attributes !!!");
            return;
        }


    }

}