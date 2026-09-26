#include "SceneHierarchyPanel.hpp"
namespace MultiStation {

	void SceneHierarchyPanel::OnImGuiRender(void) noexcept {
		ImGui::Begin("Scene Hierarchy");

		Scene& context = Application::Get().GetScene();

		context.ForEachGameObject([this](GameObject& gameobject) {
			DrawEntityNode(gameobject);
			});




		
		ImGuiPopupFlags popup_flags = ImGuiPopupFlags_MouseButtonRight |
			ImGuiPopupFlags_NoOpenOverItems;
		if (ImGui::BeginPopupContextWindow(0, popup_flags)) {
			if (ImGui::MenuItem("Create Entity")) {
				context.CreateGameObject("Game Object");
			}
			ImGui::EndPopup();
		}

		ImGui::End();
		ImGui::ShowDemoWindow();
	}

















	void SceneHierarchyPanel::DrawEntityNode(GameObject& gameobject) {
		Scene& context = Application::Get().GetScene();
		
		ImGuiTreeNodeFlags flags = 
			ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanFullWidth;
		bool isFoundSelected = false;
			bool isCntrlDown = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl);
		auto it = std::find(m_selectedGameObjects.begin(), m_selectedGameObjects.end(), &gameobject);
		if (it != m_selectedGameObjects.end()) {
			flags |= ImGuiTreeNodeFlags_Selected;
			isFoundSelected = true;
		}
		
		
		
		bool opened = ImGui::TreeNodeEx((void*)(uintptr_t)gameobject.GetID(), flags, gameobject.GetName());
		if (m_RenamingObject == &gameobject)
		{
			ImGui::SetNextItemWidth(-FLT_MIN);
			ImGui::SetKeyboardFocusHere();
			if (ImGui::InputText("rename", m_RenameBuffer, 255,
				ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			{

				gameobject.SetName(m_RenameBuffer, strnlen_s(m_RenameBuffer, 250));
				m_RenamingObject = nullptr;
			}

			if (!ImGui::IsItemActive() && (
				ImGui::IsMouseClicked(ImGuiMouseButton_Left) ||
				ImGui::IsKeyPressed(ImGuiKey_Enter)))
			{
				gameobject.SetName(m_RenameBuffer, strnlen_s(m_RenameBuffer, 250));
				m_RenamingObject = nullptr;
			}
			if (opened) {

				ImGui::TreePop();
			}
			return;
		} else if (ImGui::IsItemClicked() && ImGui::IsItemHovered() ) {
			
			if ( ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				bool isCntrlDown = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl);

				if (!isCntrlDown)
				{
					m_RenamingObject = &gameobject;
					strcpy(m_RenameBuffer, gameobject.GetName());
				}
			}
			if (!isCntrlDown) {
				m_selectedGameObjects.clear();
			}
			if (!isFoundSelected) {
				m_selectedGameObjects.push_back(&gameobject);
			}
			else if (isCntrlDown){
				m_selectedGameObjects.erase(it);
			}
			
			
			
		} 
		
		bool is_entity_deleted = false;
		ImGuiPopupFlags popup_flags = ImGuiPopupFlags_MouseButtonRight;

		if (ImGui::BeginPopupContextItem(0, popup_flags)) {
			if (ImGui::MenuItem("Delete Entity")) {

				is_entity_deleted = true;
			}
			
			ImGui::EndPopup();
		}

		if (ImGui::IsKeyDown(ImGuiKey_Delete) && (isFoundSelected)) {
			is_entity_deleted = true;
		}

		if (opened) {

			ImGui::TreePop();
		}

		if (is_entity_deleted) {
			for (GameObject* obj : m_selectedGameObjects) {
				context.RemoveGameObject(obj);
			}
			m_selectedGameObjects.clear();
			
		}
		/*if (ImGui::IsWindowFocused() == false) {
			m_selectedGameObjects.clear();
			m_RenamingObject = nullptr;
		}*/

		
	}
























	bool SceneHierarchyPanel::HasSelectedGameObject(void) const noexcept {
		return !m_selectedGameObjects.empty();
	}

	const std::vector<GameObject*> SceneHierarchyPanel::GeSelectedGameObjects(void) const noexcept {
		return m_selectedGameObjects;
	}

}
