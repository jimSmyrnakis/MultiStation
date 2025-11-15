#include "Material.hpp"

namespace MultiStation {
	


	void Material::SetShader(const char* vert, const char* frag) {
		// clear previus data
		if (m_Shader)
			delete m_Shader;
		if (m_Textures.size())
			m_Textures.clear();

		// Create new Shader
		VShader vshad(vert);
		FShader fshad(frag);
		m_Shader = new Shader(vshad, fshad);
		if (!m_Shader) {
			ASSERT(0, "No memory ");
			return;
		}


		// now find every single sampler 2D uniform and create the texture for it 
		for (int i = 0; i < m_Shader->GetUniforms()->GetDefinedUniformsCount(); i++) {
			const char* name = m_Shader->GetUniforms()->GetUniformNameById(i);
			ShaderDataType type = m_Shader->GetUniforms()->GetUniformTypeByName(name);
			int* unit = (int*)m_Shader->GetUniforms()->GetUniformPointerByName(name);
			if (type == ShaderDataType::SAMPLER_2D) {
				Texture2D* tex = new Texture2D(*unit);
				m_Textures.push_back(tex); // set up the right unit
				uint32_t width, height;
				uint32_t* dt = GetDefaultTexture2DData(&width, &height);
				Texture2DResolution res(width, height);
				tex->SetTexture(res, dt, TextureExternalFormat::RGBA8);
			}
		}


	}

	UBuffer* Material::GetUniforms(void) const {
		if (m_Shader == NULL) {
			ASSERT(0, "No Shader bound");
			return NULL;
		}

		return m_Shader->GetUniforms();
	}


	Texture2D* Material::GetTexture(int index) const {
		if (index >= m_Textures.size()) {
			ASSERT(0, "Out of Range");
			return NULL;
		}
		return m_Textures.at(index);
	}

	int Material::GetTexturesCount(void) const {
		return m_Textures.size();
	}


	void Material::Bind(void) {
		if (m_Shader) {
			m_Shader->Bind();
			for (auto Texture : m_Textures) {
				Texture->Bind();
			}
			
			
		}
	}

	void Material::Unbind(void) {
		if (m_Shader) {
			m_Shader->Unbind();
			for (auto Texture : m_Textures) {
				Texture->Unbind();
			}

		}
	}


}


