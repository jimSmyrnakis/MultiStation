#pragma once
#include "../ISerialize.hpp"
#include <unordered_map>
#include "../../Globals/Globals.hpp"


/**
 * @author Dimitris Smyrnakis
 * @file SerializationRegistry.hpp
 * @brief This file contains the Serialization Registry class , responsible for registration 
 * serialization and deserialization of Components , Systems and General Objects .  
 */
namespace MultiStation{
	
	/**
	 * @class SerializationRegistry
	 * @brief Responsible for for registration 
	 * serialization and deserialization of Components , Systems and General Objects .  
	 * Is used by the scene to serialize / deserialize all it's entities , components , systems
	 * and General Objects . This class makes it eazy a SystemModule to register for serialization 
	 * their own Components , System (Settings for the System from their point of view)  and supports
	 * extra object's for serialization (Meaning their changes remain for ever in a scene ) .
	 */
	class SerializationRegistry {
	public:
		SerializationRegistry(void) = default;
		
		/**
		 * @brief Register a component
		 * 
		 * \param id
		 * \param serialize
		 * \return 
		 */
		bool RegisterComponent(CompID id , ISerialize* serialize);

		void UnregisterComponent(CompID id);

		bool HasRegisterComponent(CompID id) const;

		bool SerializeComponent(CompID id, void* ref, IArchiveWriter* archive);

		bool DeserializeComponent(CompID id, void* ref, IArchiveReader* archive);






		bool RegisterSystem(ModuleID id,  ISerialize* serialize);

		void UnregisterSystem(ModuleID id);
		
		bool HasRegisterSystem(ModuleID id) const;
		
		bool SerializeSystem(ModuleID id, void* ref, IArchiveWriter* archive);

		bool DeserializeSystem(ModuleID id, void* ref, IArchiveReader* archive);





		bool RegisterObject(ObjID id , ISerialize* serialize);
		
		void UnregisterObject(ObjID id);

		bool HasRegisterObject(ObjID id) const;

		bool SerializeObject(ObjID id, void* ref , IArchiveWriter* archive);

		bool DeserializeObject(ObjID id, void* ref, IArchiveReader* archive);

	private:
		
		std::unordered_map<CompID, ISerialize*> m_componentsMap;
		std::unordered_map<ModuleID, ISerialize*> m_systemsMap;
		std::unordered_map<ObjID, ISerialize*> m_objectsMap;
	};

}
