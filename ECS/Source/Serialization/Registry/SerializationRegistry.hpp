#pragma once
#include "../ISerialize.hpp"
#include <unordered_map>
#include "../../Globals/Globals.hpp"

/**
 * @author Dimitris Smyrnakis
 * @file SerializationRegistry.hpp
 * @brief This file contains the Serialization Registry class , responsible for registration 
 * serialization and deserialization of Components , Systems and General Objects .  
 * @version 0.1
 */
namespace MultiStation{
	
	/**
	 * @class SerializationRegistry
	 * @brief Responsible for registration 
	 * serialization and deserialization of Components and General Objects like the systems .  
	 * Is used by the scene to serialize / deserialize all it's entities , components , systems
	 * and General Objects . This class makes it easy a Module to register for serialization 
	 * their own Components , System (Settings for the System from their point of view)  via the support
	 * for extra object's for serialization (Meaning their changes remain for ever in a scene ) .
	 */
	class SerializationRegistry {
	public:
		SerializationRegistry(void) = default;
		
		/**
		 * @brief Register for a specific component type (id) a single ISerialize interface implementation
		 * @param id The Component ID
		 * @param serialize The Serialize interface implementation .
		 */
		bool RegisterComponent(CompID id , ISerialize* serialize);

		/**
		 * @brief Register for a specific component type T a single ISerialize interface implementation
		 * @tparam T The Component class type or primitive type 
		 * @param serialize The Serialize interface implementation .
		 */
		template<typename T>
		bool RegisterComponent(ISerialize* serialize);

		/**
		 * @brief Unregisters or removes for a specific component type (id) the unique ISerialize interface
		 * that has given previus (if not register then nothing happens ) 
		 * @param id The component id 
		 */
		void UnregisterComponent(CompID id);

		/**
		 * @brief Unregisters or removes for a specific component type T the unique ISerialize interface
		 * that has given previus (if not register then nothing happens )
		 * @tparam T The component type class / primitive
		 */
		template<typename T>
		void UnregisterComponent(void);

		/**
		 * @param id The Component id
		 * @returns True if component is registered , false otherwise .
		 */
		bool HasRegisterComponent(CompID id) const;

		/**
		 * @tparam T The Component type class / primitive
		 * @returns True if component is registered , false otherwise .
		 */
		template<typename T>
		bool HasRegisterComponent(void) const;

		/**
		 * @brief Serialize (Saves) the component to the archive writer  
		 * @param id The Component id
		 * @param ref Pointer to the component data
		 * @param archive Pointer to the archive writer implementation
		 * @return True if succeded , false otherwise .
		 */
		bool SerializeComponent(CompID id, void* ref, IArchiveWriter* archive);

		/**
		 * @brief Serialize (Saves) the component to the archive writer
		 * @tparam T The Component type class / primitive
		 * @param ref Pointer to the component data
		 * @param archive Pointer to the archive writer implementation
		 * @return True if succeded , false otherwise .
		 */
		template<typename T>
		bool SerializeComponent(T* ref, IArchiveWriter* archive);

		/**
		 * @brief Deserialize (Loads) the component to the archive reader 
		 * @param id The Component id
		 * @param ref Pointer to the component data
		 * @param archive Pointer to the archive reader implementation
		 * @return True if succeded , false otherwise .
		 * @warning On Fail the given component will have probably random data .
		 */
		bool DeserializeComponent(CompID id, void* ref, IArchiveReader* archive);

		/**
		 * @brief Deserialize (Loads) the component to the archive reader
		 * @tparam T The Component type class / primitive
		 * @param ref Pointer to the component data
		 * @param archive Pointer to the archive reader implementation
		 * @return True if succeded , false otherwise .
		 * @warning On Fail the given component will have probably random data .
		 */
		template<typename T>
		bool DeserializeComponent(T* ref, IArchiveReader* archive);


		
		bool RegisterObject(ObjID id , ISerialize* serialize);
		
		void UnregisterObject(ObjID id);

		bool HasRegisterObject(ObjID id) const;

		bool SerializeObject(ObjID id, void* ref , IArchiveWriter* archive);

		bool DeserializeObject(ObjID id, void* ref, IArchiveReader* archive);

	private:
		
		std::unordered_map<CompID, ISerialize*> m_componentsMap;
		std::unordered_map<ObjID, ISerialize*> m_objectsMap;
	};




	template<typename T>
	bool SerializationRegistry::RegisterComponent(ISerialize* serialize) {
		return RegisterComponent(GetComponentID<T>(), serialize);
	}

	template<typename T>
	void SerializationRegistry::UnregisterComponent(void) {
		UnregisterComponent(GetComponentID<T>());
	}

	template<typename T>
	bool SerializationRegistry::HasRegisterComponent(void) const {
		return HasRegisterComponent(GetComponentID<T>());
	}



	template<typename T>
	bool SerializationRegistry::SerializeComponent(T* ref, IArchiveWriter* archive) {
		return SerializeComponent(GetComponentID<T>(), ref, archive);
	}


	template<typename T>
	bool SerializationRegistry::DeserializeComponent(T* ref, IArchiveReader* archive) {
		return DeserializeComponent(GetComponentID<T>(), ref, archive);
	}

}
