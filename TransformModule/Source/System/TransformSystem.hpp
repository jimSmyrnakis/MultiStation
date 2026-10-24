#pragma once

#include <ECS.hpp>

namespace MultiStation {

	class TransformSystem : public ISystem {

	public:

		TransformSystem(EngineContext& ctx);

		void OnAttach(void) noexcept override;

		void OnDettach(void) noexcept override;

		void OnUpdate(float dt) noexcept override;


	private:
		EngineContext& m_context;
	};


}
