#pragma once
#include "../Components/EntityComponent.hpp"

namespace MultiStation {

	class InspectorPanel : public IDrawable {

	public:
		InspectorPanel(void) noexcept;


		void Draw(void) noexcept override;

		void SetEngineContext(EngineContext& ctx) noexcept;

		void SetSelectedEntity(EntID entity) noexcept;

		EntID GetSelectedEntity(void) const noexcept;

	private:

		void DrawElements(void) noexcept;
	private:
		EntID m_SelectedEntity = nullEntity;
		EngineContext* m_context;
		static EntityComponentInspector s_entityComponentInspector;

	};



}
