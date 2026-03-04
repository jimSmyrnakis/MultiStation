#pragma once
#include <ECS.hpp>
#include <Application.hpp>
#include <ImGui.hpp>
#include <Components.hpp>
namespace MultiStation{
	class HierarchyPanel {
	public:
		HierarchyPanel(void);

		void OnImGuiRender(void) noexcept;

	private:
		void DrawEntityNode(GameObject& gameobject );

	private:
		GameObject* m_SelectedEntityId = nullptr;
	};
}
