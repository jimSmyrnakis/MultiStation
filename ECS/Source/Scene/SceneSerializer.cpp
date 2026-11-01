#include "SceneSerializer.hpp"

namespace MultiStation {

	SceneSerializer::SceneSerializer(SerializationRegistry& registry) : m_registry(registry) {

	}

	bool SceneSerializer::Serialize(void* ref, IArchiveWriter* archive) {
		Scene* scene = static_cast<Scene*>(ref);
		if (!ref || !archive) {
			return false;
		}
		auto Ents = scene->GetAllEntities();
		archive->BeginObject("Basic Info");
		archive->WriteField("Entities", (int)Ents.size());
		archive->EndObject();
		// for all entities
		for (EntID ent : Ents) {
			archive->BeginObject("Entity");
			// get All components
			{
				auto Comps = scene->GetEntityComponentsIDs(ent);
				archive->WriteField("Count Components", (int)Comps.size() );
				int i = 0;
				std::vector<CompID> register_ids;
				// for each component id save the name of each component
				for (CompID comp : Comps) {
					std::string field = "Component" + std::to_string(i);
					archive->WriteField(field, scene->GetName(comp));
					bool has_it = m_registry.HasRegisterComponent(comp);
					archive->WriteField("Saved", has_it ? 1 : 0);
					if (has_it) register_ids.push_back(comp);

					i++;
				}


				// now write each register (for save) id
				for (CompID comp : register_ids) {
					void* data = scene->GetComponent( comp, ent);
					m_registry.SerializeComponent(comp ,data ,  archive );
				}

			}
			archive->EndObject();
		}
		
		return true;
	}

	bool SceneSerializer::Deserialize(void* ref, IArchiveReader* archive) {
		Scene* scene = static_cast<Scene*>(ref);
		if (!ref || !archive) {
			return false;
		}


		int entities = 0;
		archive->BeginObject("Basic Info");
		archive->ReadField("Entities", entities);
		archive->EndObject();
		// for all entities
		for (int i = 0; i < entities; i++) {
			archive->BeginObject("Entity");
			// Create Entitty
			EntID ent = scene->CreateEntity();
			// get All entities components
			{
				int components = 0;
				archive->ReadField("Count Components", components);
				std::vector<std::string> names;
				std::vector<int> saved;
				// for each component take the names and if saved
				for (int j = 0; j < components; j++) {
					
					std::string field = "Component" + std::to_string(j);
					names.push_back({});
					archive->ReadField(field, names[j]);

					int has_it = 0;
					archive->ReadField("Saved", has_it );

					saved.push_back(has_it);
				}


				// now read each saved component
				for (int j = 0; j < components; j++) {
					if (!saved[j]) continue;
					// Get component id from name
					CompID comp = scene->GetComponentIDByName(names[j]);
					// Create entity component
					void* data = scene->AddComponent(ent, comp);

					m_registry.DeserializeComponent(comp, data, archive);
				}

			}
			archive->EndObject();
		}

		return true;
	}

}
