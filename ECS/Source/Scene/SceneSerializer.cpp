#include "SceneSerializer.hpp"

namespace MultiStation {

	

	bool SceneSerializer::Serialize(void* ref, IArchiveWriter* archive) {
		Scene* scene = static_cast<Scene*>(ref);
		if (!ref || !archive) {
			return false;
		}

		auto rootEnts = scene->m_sceneGraph.GetChilds(rootEntity);
		// to be implemented with dfs in the future ...
		
		return true;
	}

	bool SceneSerializer::Deserialize(void* ref, IArchiveReader* archive) {
		Scene* scene = static_cast<Scene*>(ref);
		if (!ref || !archive) {
			return false;
		}

		// to be implemented with dfs in the future ...

		return true;
	}

}
