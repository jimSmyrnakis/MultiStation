#pragma once
#include "Scene.hpp"
#include "SceneSerializer.hpp"
#include "../Serialization/Archive/TextArchive.hpp"
#include <Utilities.hpp>
namespace MultiStation{

	class SceneManager {

	private:
		enum CMDType {
			CREATE , 
			DESTROY ,
			ACTIVATE,
			DEACTIVATE
		};

	public:
		
		SceneManager(EngineContext& ctx  , ObjectMap& objMap , SerializationRegistry& registry);
		~SceneManager(void) noexcept;

		void Init(void);

		void Fini(void);

		/**
		 * @brief Returns if exist's the current scene that is loaded .
		 */
		Scene* GetCurrentScene(void);


		void ProcessPendingCommands(void) noexcept;


	private:
		Scene* m_currentScene;
		EngineContext& m_context;
		ObjectMap& m_objectMap;
		SerializationRegistry& m_serializer;
		static SceneSerializer* s_sceneSerializer;
		IArchiveReader* m_reader;
		IArchiveWriter* m_writer;
		int m_fhnd;
		void* m_buffer;
		size_t m_count;
	};

}
