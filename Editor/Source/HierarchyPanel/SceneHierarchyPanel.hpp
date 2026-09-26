#pragma once
#include <SceneManager.hpp>
#include <Application.hpp>
#include <ImGui.hpp>
#include <Components.hpp>
namespace MultiStation {
	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel(void) noexcept = default;

		void OnImGuiRender(void) noexcept;

		bool HasSelectedGameObject(void) const noexcept;

		const std::vector<GameObject*> GeSelectedGameObjects(void) const noexcept;

	private:
		void DrawEntityNode(GameObject& gameobject);

	private:
		std::vector<GameObject*> m_selectedGameObjects;
		GameObject* m_RenamingObject = nullptr;
		char m_RenameBuffer[256];
	};
}
