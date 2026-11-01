#pragma once
#include "../Serialization/Registry/SerializationRegistry.hpp"
#include "Scene.hpp"
namespace MultiStation{

	class SceneSerializer : public ISerialize {

	public:
		
		SceneSerializer(SerializationRegistry& registry);

		bool Serialize(void* ref , IArchiveWriter* archive) override;
		bool Deserialize(void* ref , IArchiveReader* archive) override;

	private:
		SerializationRegistry& m_registry;
	};

}
