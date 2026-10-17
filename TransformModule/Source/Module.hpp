#pragma once
#include <ECS.hpp>
#include "System/TransformSystem.hpp"
namespace MultiStation{

	class Transform3DModule : public ISystemModule {
	public:

		void Init(EngineContext& context) override;

		
		void Fini(EngineContext& context) override;

		
		ISystem& GetSystem(void) override;

	private:
		TransformSystem* m_system;
	};

}
