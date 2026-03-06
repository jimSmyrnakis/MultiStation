#include "HierarchyPanel.hpp"
#include <sstream>
namespace MultiStation {

	HierarchyPanel::HierarchyPanel(void) {
		m_SelectedEntityId = nullptr;
		Scene& context = Application::Get().GetScene();
		if (context.HasRegister<int>() == false) {
			context.Register<int>();
		}
	}

	void HierarchyPanel::OnImGuiRender(void) noexcept {
		ImGui::Begin("Scene Hierarchy");

		Scene& context = Application::Get().GetScene();
		
		std::vector<GameObject>& GameObjects = context.GetGameObjects();
		uint32_t min_len = std::min((size_t)100, GameObjects.size());
		for (int i = 0; i < min_len; i++) {
			GameObject& gameobject = GameObjects[i];
			DrawEntityNode(gameobject);
		}
		std::vector<int>& ints = context.GetComponents<int>();
		for (int& i : ints) {
			i++;
		}
		/* for (size_t i = min_len; i < GameObjects.size(); i++) {
			GameObject& gameobject = GameObjects[i];
			//DrawEntityNode(gameobject);
			const char* name = gameobject.GetName();
			if (gameobject.HasComponent<int>()){
				int* ptr = gameobject.GetComponent<int>();
				(*ptr)++;
			}
				
		}*/
		bool atAnyLRClick = ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right);

		if  ( (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
		|| (!ImGui::IsWindowFocused()) ){
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
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_SpanFullWidth;
		if (&gameobject == m_SelectedEntityId)
			flags |= ImGuiTreeNodeFlags_Selected;
		char buffer[128];
		snprintf(buffer, sizeof(buffer), "%s %u", gameobject.GetName(), gameobject.GetID());
		bool opened = ImGui::TreeNodeEx((void*)(uintptr_t)gameobject.GetID(), flags, buffer);
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
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
				| ImGuiTreeNodeFlags_SpanFullWidth;
			bool opened = ImGui::TreeNodeEx((void*)(uintptr_t)gameobject.GetID(), flags, buffer);
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (is_entity_deleted) {
			context.RemoveGameObject(&gameobject);
		}
			
	}

}
