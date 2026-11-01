#include "SceneManager.hpp"
#include "../Streams/FileStream.hpp"
#include "../Serialization/Archive/TextArchive.hpp"
#include <IO.hpp>
namespace MultiStation {
	SceneSerializer* SceneManager::s_sceneSerializer = nullptr;
	
	SceneManager::SceneManager(EngineContext& ctx , ObjectMap& objMap , SerializationRegistry& registry)
		: m_context(ctx)  , m_objectMap(objMap) , m_serializer(registry) {
		m_currentScene = new Scene(m_context, m_objectMap);
		
		m_context.GetCurrentScene = [this]() -> Scene* {
			return m_currentScene;
		};

		if (s_sceneSerializer == nullptr)
			s_sceneSerializer = new SceneSerializer(m_serializer);
		
		m_reader = new TextArchiveReader();
		m_writer = new TextArchiveWriter();
		m_count = 1024 * 1024 * 16;
		m_buffer = malloc(m_count);
		
	}

	SceneManager::~SceneManager(void) noexcept {
		delete m_currentScene;
	}

	void SceneManager::ProcessPendingCommands(void) noexcept {
		// not implemented yet
	}


	/**
	 * @brief Returns if exist's the current scene that is loaded .
	 */
	Scene* SceneManager::GetCurrentScene(void) {
		return m_currentScene;
	}


	void SceneManager::Init(void) {
		
		
		// Try Open file Scene.txt
		bool first_time = false;
		ErrorCode res = IO::Open("./Scene.txt", IO::Access::READ , m_fhnd);
		while (res != ErrorCode::SUCCESS ) {
			if (res == ErrorCode::FILE_NOT_EXISTS) {
				IO::Create("./Scene.txt", false);
				first_time = true;
			}
			else {
				MS_ASSERT(false, "Oh no");
			}
			res = IO::Open("./Scene.txt", IO::Access::READ , m_fhnd);
		}

		if (first_time)
			return;
		uint32_t readed = 0;
		bool eof = false;
		IO::Read(m_fhnd, m_buffer, m_count, readed, eof);
		printf("\n\nreaded: %d\n\n", readed);
		m_reader->SetBuffer(m_buffer, readed);
		s_sceneSerializer->Deserialize(m_currentScene, m_reader);
		IO::Close(m_fhnd);
	}

	void SceneManager::Fini(void){
		ErrorCode res = IO::Open("./Scene.txt", IO::Access::WRITE, m_fhnd);
		s_sceneSerializer->Serialize(m_currentScene, m_writer);
		uint32_t writed;
		IO::Write(m_fhnd, m_writer->GetBuffer(), m_writer->GetCount(), writed);
		IO::Close(m_fhnd);
	}


}
