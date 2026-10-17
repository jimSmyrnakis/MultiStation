#pragma once
#include <ECS.hpp>
#include "System/Renderer3DDS.hpp"
namespace MultiStation{

	class Renderer3DDSModule : public ISystemModule {
	public:
		void Init(EngineContext& context) override;
		 

		void Fini(EngineContext& context) override;


		ISystem& GetSystem(void) override;

	private:
		Renderer3DDS* m_system;
	};

}
