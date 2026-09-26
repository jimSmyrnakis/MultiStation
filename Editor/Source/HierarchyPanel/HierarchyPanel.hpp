#pragma once
#include <SceneManager.hpp>
#include <Application.hpp>
#include <ImGui.hpp>
#include <Components.hpp>
namespace MultiStation{
	class HierarchyPanel {
	public:
		HierarchyPanel(void);

		void OnImGuiRender(void) noexcept;

		bool HasSelectedGameObject(void) const noexcept;

		GameObject* GeSelectedGameObject(void) const noexcept;

	private:
		void DrawEntityNode(GameObject& gameobject );

	private:
		GameObject* m_SelectedEntityId = nullptr;
	};
}
