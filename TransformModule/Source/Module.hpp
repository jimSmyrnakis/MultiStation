#pragma once
#include <ECS.hpp>
#include "System/TransformSystem.hpp"
namespace MultiStation{

	class Transform3DModule : public ISystemModule {
	public:

		void Init(EngineContext& context) noexcept override;

		
		void Fini(EngineContext& context) noexcept override;

		
		ISystem& GetSystem(void) noexcept override;

	private:
		TransformSystem* m_system;
	};

}
