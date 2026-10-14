#include "SceneManager.hpp"
#include "../Streams/FileStream.hpp"
#include "../Serialization/Archive/TextArchive.hpp"
namespace MultiStation {

	
	SceneManager::SceneManager(EngineContext& ctx , SerializationRegistry& serializer, ObjectMap& objMap)
		: m_context(ctx) , m_serializer(serializer) , m_objectMap(objMap) {
		m_currentScene = new Scene(m_context, serializer, m_objectMap);
		m_serializer.RegisterObject(GetObjectID<Scene>(), &GetSceneSerializer());
		m_context.GetCurrentScene = [this]() -> Scene* {
			return m_currentScene;
		};
		m_context.LoadScene = [this](IArchiveReader* archiveReader) -> bool {
			return LoadScene(archiveReader);
		};
		m_context.SaveScene = [this](IArchiveWriter* archiveWriter) -> bool {
			return SaveScene(archiveWriter);
		};


		
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

		m_currentScene = new Scene(m_context, m_serializer , m_objectMap);
		if (!m_currentScene) {
			return false;
		}

		

		if (!m_serializer.
			DeserializeObject(GetObjectID<Scene>(), m_currentScene, archiveReader)) {
			
			return false;
		}
		delete m_currentScene;
		m_currentScene = nullptr;
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
