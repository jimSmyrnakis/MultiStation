#pragma once

#include <ECS.hpp>

namespace MultiStation {

	class TransformSystem : public ISystem {

	public:

		TransformSystem(EngineContext& ctx);

		void OnAttach(void) override;

		void OnDettach(void) override;

		void OnUpdate(float dt) override;

		void OnEvent(Event& e) override;

	private:
		EngineContext& m_context;
	};


}
