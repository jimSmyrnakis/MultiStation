#pragma once
#include "Scene.hpp"
namespace MultiStation{

	class SceneManager {

	public:
		/**
		 * @brief Creates / Contructs the SceneManager and takes to move down the engine context .
		 * 
		 * @param ctx The Engine context
		 */
		SceneManager(EngineContext& ctx , SerializationRegistry& serializer);




		/**
		 * @brief Returns if exist's the current scene that is loaded .
		 */
		Scene* GetCurrentScene(void);

		/**
		 * @brief Loads the new scene from the given archive .
		 * @param archiveReader The archive to load the scene .
		 * @return True if scene loaded succesfully or false otherwise . 
		 */
		bool LoadScene(IArchiveReader* archiveReader);

		/**
		 * 
		 * @brief Saves the scene to a given archive .
		 * @param archiveWriter The archive where the scene will use to save .
		 * @return True if the scene saved succesfully otherwise false . 
		 */
		bool SaveScene(IArchiveWriter* archiveWriter);




	private:
		Scene* m_currentScene;
		EngineContext& m_context;
		SerializationRegistry& m_serializer;
		
		
	};

}
