#include "Material.hpp"
#include <string.h>

#define MAX_TEXTURES_COUNT 16

namespace Game{

    bool IsInSamplers2D(UniformType type){
        switch (type){
            case UniformType::SAMPLER_2D    :  
            case UniformType::ISAMPLER_2D   :
            case UniformType::USAMPLER_2D   :   return true ;
            default                         :   return false;
        }
        return false; 
    }

    TextureInternalFormat GetSamplers2DRes(Material2D::Format matFormat , UniformType type){
        //return TextureInternalFormat::RGBA16;
        
        if (type == UniformType::SAMPLER_2D){
            switch(matFormat){
                case Material2D::Format::RGB4   :   return TextureInternalFormat::RGB4  ;
                case Material2D::Format::RGB8   :   return TextureInternalFormat::RGB8  ;
                case Material2D::Format::RGB16  :   return TextureInternalFormat::RGB16 ;
                case Material2D::Format::RGBA4  :   return TextureInternalFormat::RGBA4 ;
                case Material2D::Format::RGBA8  :   return TextureInternalFormat::RGBA8 ;
                case Material2D::Format::RGBA16 :   return TextureInternalFormat::RGBA16;
                default                         :   break;
            }
        }

        if (type == UniformType::ISAMPLER_2D){
            switch(matFormat){
                case Material2D::Format::RGB4   :   return TextureInternalFormat::RGB4I  ;
                case Material2D::Format::RGB8   :   return TextureInternalFormat::RGB8I  ;
                case Material2D::Format::RGB16  :   return TextureInternalFormat::RGB16I ;
                case Material2D::Format::RGBA4  :   return TextureInternalFormat::RGBA4I ;
                case Material2D::Format::RGBA8  :   return TextureInternalFormat::RGBA8I ;
                case Material2D::Format::RGBA16 :   return TextureInternalFormat::RGBA16I;
                default                         :   break;
            }
        }

        if (type == UniformType::USAMPLER_2D){
            switch(matFormat){
                case Material2D::Format::RGB4   :   return TextureInternalFormat::RGB4UI  ;
                case Material2D::Format::RGB8   :   return TextureInternalFormat::RGB8UI  ;
                case Material2D::Format::RGB16  :   return TextureInternalFormat::RGB16UI ;
                case Material2D::Format::RGBA4  :   return TextureInternalFormat::RGBA4UI ;
                case Material2D::Format::RGBA8  :   return TextureInternalFormat::RGBA8UI ;
                case Material2D::Format::RGBA16 :   return TextureInternalFormat::RGBA16UI;
                default                         :   break;
            }
        }
        ASSERT(0 , "Undefined Format and uniform type for material !!!");
        return TextureInternalFormat::RGBA16;
    }

    void Material2D::InitMaterials(void){
        m_Count = 0;
        m_SamplerNames = nullptr;
        m_Textures = nullptr;
        m_TextureSlots = nullptr;
        if (m_Shader->GetUniforms() == nullptr){
            // no uniforms means no materials , that doesnt mean we have error just no textures
            return;
        }

        //take the number of uniforms in total
        u32 TotalUniforms = m_Shader->GetUniforms()->GetUniformsCount();
        if (TotalUniforms == 0){
            // no uniforms means no materials , that doesnt mean we have error just no textures
            return ;
        }

        //create that many string's for the names to be cached
        m_SamplerNames = (char**)m_Malloc(TotalUniforms * sizeof(char*));
        if (m_SamplerNames == nullptr){
            ASSERT(0 , "No memory !!!");
            return;
        }

        //find max uniform name
        i32 maxUSize = m_Shader->MaxUniformNameLength();
        if (maxUSize <= 0){
            ASSERT(0 , "Something is Rong !!! :(");
            m_Free(m_SamplerNames);
            m_SamplerNames = nullptr;
            return;
        }

        //allocate all space needed for the names in contigues memory for better caching (not that good mechanism but is better from nothing)
        m_SamplerNames[0] = (char*)m_Malloc(TotalUniforms* maxUSize + maxUSize); // plus extra one byte for the null terminated character
        if (m_SamplerNames[0] == nullptr){
            ASSERT(0 , "No Memory !!!");
            m_Free(m_SamplerNames);
            m_SamplerNames = nullptr;
            return;
        }

        //set each string to point in the right place for the text that belongs to it and store
        for (int i =1 ;i < TotalUniforms; i++){
            m_SamplerNames[i] = m_SamplerNames[i-1] + maxUSize + i;
            m_SamplerNames[i][maxUSize] = '\0';// for be sure
            
        }

        //for each uniform , find if is in the special uniform samplers 2D 
        char* maxName = (char*)m_Malloc(maxUSize + 1);
        maxName[maxUSize] = '\0';
        for (int i = 0; i < TotalUniforms; i++){
            // take the name of the uniform
            m_Shader->GetUniforms()->GetUniformNameByIndex(i , maxName , maxUSize);
            // based on the name take the type
            if ( !( IsInSamplers2D(m_Shader->GetUniforms()->GetUniformTypeByName(maxName)) ) ) continue;
                // if not a sampler2D element then do nothing
                
            //else record his name to the sampler's names and increment the m_Count
            m_Count++;
            strncpy(m_SamplerNames[m_Count - 1] , maxName , maxUSize +  1);
        }
        // now free the memory of the maxName variable
        m_Free(maxName);

        //if the m_Count is zero or bigger than the maximum texture slots then free all the memory from the m_Samplers name and don't keep going
        if ( (m_Count == 0) || (m_Count > MAX_TEXTURES_COUNT) ) {
            ASSERT(m_Count <= MAX_TEXTURES_COUNT , "Too many texture samplers !!!" );
            m_Free(m_SamplerNames[0]);
            m_Free(m_SamplerNames);
            m_SamplerNames = nullptr;
            m_SamplerNames[0] = nullptr;
            return;
        }

        // if comes here means we have found sampler2D type uniform variables and so we need to create the texture slots
        m_TextureSlots = (u32*)m_Malloc(m_Count* sizeof(u32));
        if (m_TextureSlots == nullptr){
            ASSERT(0 , "No Memory !!!");
            // free samplers names too and return 
            m_Free(m_SamplerNames[0]);
            m_Free(m_SamplerNames);
            m_SamplerNames = nullptr;
            m_SamplerNames[0] = nullptr;
            return;
        }

        // if still memory is successfully allocated now go and create slots
        for(int i = 0; i < m_Count; i++){
            m_TextureSlots[i] = i;
        }

        //Allocate m_Count textures
        m_Textures = (Texture2D**)m_Malloc(m_Count * sizeof(Texture2D*));

        //check if memory is allocated
        if (m_Textures == nullptr){
            ASSERT(0 , "No memory !!!");
            m_Free(m_SamplerNames[0]);
            m_Free(m_SamplerNames); // this time free the texture slots too
            m_Free(m_TextureSlots);
            m_SamplerNames = nullptr;
            m_SamplerNames[0] = nullptr;
            m_TextureSlots = nullptr;
            return;
        }

        //if everything is ok go and create each texture with the desired internal format
        bool memory = true;
        for (int i =0; i< m_Count; i++){
            TextureInternalFormat iform;
            UniformType type;
            type = m_Shader->GetUniforms()->GetUniformTypeByName(m_SamplerNames[i] );
            iform = GetSamplers2DRes(m_CommonFormat.GetFormat() ,type );
            
            Texture2DResolution res(m_CommonFormat.GetWidth() , m_CommonFormat.GetHeight() , iform);
            m_Textures[i] = new Texture2D(m_TextureSlots[i] , res); //TODO -- make it with the m_Malloc not new 
            if (m_Textures[i] == nullptr){
                //if no memory
                ASSERT(0 , "No Memory !!!");
                for (int j = 0 ; j < i ; j++)
                    delete m_Textures[j];
                memory = false;
                break;
            }
        }

        if (memory == false){
            ASSERT(0 , "No memory !!!");
            m_Free(m_SamplerNames[0]);
            m_Free(m_SamplerNames); 
            m_Free(m_TextureSlots);
            m_Free(m_Textures);
            m_SamplerNames = nullptr;
            m_SamplerNames[0] = nullptr;
            m_TextureSlots = nullptr;
            m_Textures = nullptr;
            return;
        }
    }
}