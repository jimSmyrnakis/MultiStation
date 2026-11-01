#include "SerializationRegistry.hpp"

namespace MultiStation {




	bool SerializationRegistry::RegisterComponent(CompID id, ISerialize* serialize) {
		if ((serialize == nullptr)  ) 
			return false;

		m_componentsMap[id] = serialize;
		return true;
	}

	bool SerializationRegistry::RegisterObject(ObjID id, ISerialize* serialize) {
		if ((serialize == nullptr))
			return false;

		m_objectsMap[id] = serialize;
		return true;
	}


	void SerializationRegistry::UnregisterComponent(CompID id) {
		m_componentsMap.erase(id);
	}

	void SerializationRegistry::UnregisterObject(ObjID id) {
		m_objectsMap.erase(id);
	}



	bool SerializationRegistry::HasRegisterComponent(CompID id) const {
		return m_componentsMap.count(id) != 0;
	}


	bool SerializationRegistry::HasRegisterObject(ObjID id) const {
		return m_objectsMap.count(id) != 0;
	}




	bool SerializationRegistry::SerializeComponent(CompID id, void* ref, IArchiveWriter* archive) {
		if (!HasRegisterComponent(id) || archive == nullptr || ref == nullptr)
			return false;

		ISerialize* serialize = m_componentsMap[id];
		return serialize->Serialize(ref, archive);
	}


	


	bool SerializationRegistry::SerializeObject(ObjID id, void* ref, IArchiveWriter* archive) {
		if (!HasRegisterObject(id) || archive == nullptr || ref == nullptr)
			return false;

		ISerialize* serialize = m_objectsMap[id];
		return serialize->Serialize(ref, archive);
	}




	bool SerializationRegistry::DeserializeComponent(CompID id, void* ref, IArchiveReader* archive) {
		if (!HasRegisterComponent(id) || archive == nullptr || ref == nullptr)
			return false;
		ISerialize* serialize = m_componentsMap[id];
		
		return serialize->Deserialize(ref, archive);;
	}


	bool SerializationRegistry::DeserializeObject(ObjID id, void* ref, IArchiveReader* archive) {
		if (!HasRegisterObject(id) || archive == nullptr || ref == nullptr)
			return false;

		ISerialize* serialize = m_objectsMap[id];
		return serialize->Deserialize(ref, archive);
		
	}

}
