#ifndef MULTISTATION_RENDERER3DDS_MODEL_H
#define MULTISTATION_RENDERER3DDS_MODEL_H

#include "../../Defs.hpp"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include "../GraphicsApi/VBuffer/VLayout.hpp"
#include "../GraphicsApi/VBuffer/VBuffer.hpp"
namespace MultiStation {

	struct MeshData {
		float* positions;
		float* normals;
		float* texCords;
		uint32_t* indices;

		uint32_t vertexCount;   // πόσα vertices
		uint32_t indexCount;    // πόσα indices
	};

	class Model {
	public:

		

		void LoadModel(std::string& path);
		VBuffer* GetVBuffers(void) const;
		uint32_t GetVBuffersCount(void) const;
	private:
		void DestroyMesh(void);
		void Load(void);
		
		std::vector<struct MeshData> m_Meshes;
		std::vector<VertexLayout> m_Layouts;
		VBuffer* m_VBuffers;
	};
}

#endif 
