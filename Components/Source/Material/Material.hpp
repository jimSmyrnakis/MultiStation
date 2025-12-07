#pragma once
#include "../GraphicsApi/Shader/Shader.hpp"
#include "../GraphicsApi/UBuffer/UBuffer.hpp"
#include "../GraphicsApi/Texture/Texture2D.hpp"
#include "Core.hpp"

namespace MultiStation{
	class Material {
	public:

		
		void SetShader(const char* vert , const char* frag);
		UBuffer* GetUniforms(void) const;
		Texture2D* GetTexture(int index) const;
		int GetTexturesCount(void) const;


		void Bind(void);
		void Unbind(void);


	private:
		Shader* m_Shader;
		std::vector<Texture2D*> m_Textures;
	};
}
