#include "InspectorPanel.hpp"

namespace MultiStation {


	InspectorPanel::InspectorPanel(void) noexcept  : IDrawable() {
		m_SelectedEntity = nullEntity;
		m_context = nullptr;
		Editor::RegisterInspectable<EntityComponent>(s_entityComponentInspector);
	}


	void InspectorPanel::Draw(void) noexcept {
		ImGui::Begin("Inspector");
		DrawElements();
		ImGui::End();
	}

	void InspectorPanel::SetEngineContext(EngineContext& ctx) noexcept {
		m_context = &ctx;
	}

	void InspectorPanel::SetSelectedEntity(EntID entity) noexcept {
		m_SelectedEntity = 
			entity;
	}

	EntID InspectorPanel::GetSelectedEntity(void) const noexcept {
		return m_SelectedEntity;
	}

	void InspectorPanel::DrawElements(void) noexcept {
		// if there is no context or no selected entity, we don't draw anything
		if (m_context == nullptr || m_SelectedEntity == nullEntity) {
			return;
		}

		// now take the selected entity and draw its Entity Components
		// 1. take from scene the component of this entity with type EntityComponent
		void* data =
			(void*)m_context->GetCurrentScene()->GetComponent<EntityComponent>(m_SelectedEntity);
		//2. Take the IInspectable interface from the Editor
		IInspectable* inspectable = Editor::GetInspectable(GetObjectID<EntityComponent>());
		if (data && inspectable) {
			
			inspectable->DrawElement(data);
			
			
		}
	}

	EntityComponentInspector InspectorPanel::s_entityComponentInspector;

}
