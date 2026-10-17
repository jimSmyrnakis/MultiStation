#pragma once
#include <Common.hpp>

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
		void DrawAddComponentButton(void);
	private:
		EntID m_SelectedEntity = nullEntity;
		EngineContext* m_context;
		static Component::EntityInspectable s_entityComponentInspector;
		static Component::Transform3DInspectable s_entityTransformInspector;
	};



}
