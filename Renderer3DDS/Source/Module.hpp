#pragma once

#include "System/Renderer3DDS.hpp"
#include <ECS.hpp>
namespace MultiStation{

	class Renderer3DDSModule : public ISystemModule {
	public:
		void Init(EngineContext& context) noexcept override;
		 

		void Fini(EngineContext& context) noexcept override;


		ISystem& GetSystem(void) noexcept override;

	private:
		Renderer3DDS* m_system;
	};

}
