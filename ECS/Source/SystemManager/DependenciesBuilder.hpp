#pragma once
#include "../ISystem/ISystem.hpp"
#include "../Task/ITaskMetadata.hpp"
#include "../Task/Schedulers/Pipeline/Pipeline.hpp"
namespace MultiStation {

	class DependenciesBuilder {
	public:
		DependenciesBuilder(void);

		void AddSystem(ISystem* system);
		void RemoveSystem(ISystem* system);
		std::unordered_map<uint32_t, std::vector<ISystem*>>&
			GetSystemOrder(void);

	private:
		void CaculateDependencies(void);
		

	private:
		std::vector<ISystem*> m_systems;
		std::unordered_map<uint32_t, std::vector<ISystem*>> m_pipeline;
		bool m_hasChanged = false;
	};

}
