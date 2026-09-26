#include "HierarchyPanel.hpp"
#include <sstream>
namespace MultiStation {

	HierarchyPanel::HierarchyPanel(void) {
		m_SelectedEntityId = nullptr;
		Scene& context = Application::Get().GetScene();

	}

	void HierarchyPanel::OnImGuiRender(void) noexcept {
		ImGui::Begin("Scene Hierarchy");

		Scene& context = Application::Get().GetScene();

		context.ForEachGameObject([this](GameObject& gameobject) {
			DrawEntityNode(gameobject);
			});




		bool atAnyLRClick = ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right);

		if ((ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
			|| (!ImGui::IsWindowFocused())) {
			m_SelectedEntityId = 0;
		}
		ImGuiPopupFlags popup_flags = ImGuiPopupFlags_MouseButtonRight | 
			ImGuiPopupFlags_NoOpenOverItems ;
		if (ImGui::BeginPopupContextWindow(0 , popup_flags)) {
			if (ImGui::MenuItem("Create Entity")) {
				context.CreateGameObject("Game Object");
			}
			ImGui::EndPopup();
		}

		ImGui::End();
		//ImGui::ShowDemoWindow();
	}

	void HierarchyPanel::DrawEntityNode(GameObject& gameobject) {
		Scene& context = Application::Get().GetScene();
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Bullet
			| ImGuiTreeNodeFlags_SpanFullWidth;
		if (&gameobject == m_SelectedEntityId)
			flags |= ImGuiTreeNodeFlags_Selected;

		bool opened = ImGui::TreeNodeEx((void*)(uintptr_t)gameobject.GetID(), flags, gameobject.GetName());
		if (ImGui::IsItemClicked()) {
			m_SelectedEntityId = &gameobject;
			// TODO - Show the entity's components in the properties panel
		}
		bool is_entity_deleted = false;
		ImGuiPopupFlags popup_flags = ImGuiPopupFlags_MouseButtonRight;
			
		if (ImGui::BeginPopupContextItem(0, popup_flags)) {
			if (ImGui::MenuItem("Delete Entity")) {

				is_entity_deleted = true;
			}
			ImGui::EndPopup();
		}

		if (ImGui::IsKeyDown(ImGuiKey_Delete) && (m_SelectedEntityId == &gameobject)) {
			is_entity_deleted = true;
		}
		
		if (opened) {
			
			ImGui::TreePop();
		}

		if (is_entity_deleted) {
			context.RemoveGameObject(&gameobject);
		}
			
	}


	bool HierarchyPanel::HasSelectedGameObject(void) const noexcept {
		return m_SelectedEntityId != nullptr;
	}

	GameObject* HierarchyPanel::GeSelectedGameObject(void) const noexcept {
		return m_SelectedEntityId;
	}

}
