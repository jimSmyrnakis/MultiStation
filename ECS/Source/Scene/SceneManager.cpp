#include "SceneManager.hpp"
#include "../Streams/FileStream.hpp"
#include "../Serialization/Archive/TextArchive.hpp"
namespace MultiStation {

	
	SceneManager::SceneManager(EngineContext& ctx , SerializationRegistry& serializer) 
		: m_context(ctx) , m_serializer(serializer) {
		
		m_currentScene = nullptr;
		m_serializer.RegisterObject(GetObjectID<Scene>() , &GetSceneSerializer());
	}




	/**
	 * @brief Returns if exist's the current scene that is loaded .
	 */
	Scene* SceneManager::GetCurrentScene(void) {
		return m_currentScene;
	}


	
	bool SceneManager::LoadScene(IArchiveReader* archiveReader) {
		
		if ( !archiveReader) {
			return false;
		}

		if (m_currentScene) {
			delete m_currentScene;
		}

		m_currentScene = new Scene(m_context, m_serializer);
		if (!m_currentScene) {
			return false;
		}

		

		if (m_serializer.
			DeserializeObject(GetObjectID<Scene>(), m_currentScene, archiveReader)) {
			delete m_currentScene;
			m_currentScene = nullptr;
			return false;
		}

		return true;
	}

	
	bool SceneManager::SaveScene(IArchiveWriter* archiveWriter) {

		if (!m_currentScene || !archiveWriter) {

			return false;
		}


		if (
			!m_serializer.SerializeObject(GetObjectID<Scene>() , m_currentScene , archiveWriter)
			) {
			return false;
		}


		return true;
	}


}
