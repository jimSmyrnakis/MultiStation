#include "SceneHierarchyController.hpp"

namespace MultiStation {

	SceneHierarchyController::SceneHierarchyController(
		SceneHierarchyPanel& view,
		InspectorPanel& inspectorPanel,
		EngineContext& context) noexcept
		: m_view(view), m_context(context) , m_inspectorPanel(inspectorPanel) {


		view.SetActionRightClick([this]() {
				Scene* scene = m_context.GetCurrentScene();

				SceneHierarchyAction action = m_view.GetLastAction();

				switch (action) {
				case SceneHierarchyAction::CreateChildEntity: {
					EntID selectedEntity = m_view.GetSelectedEntity();
					EntID newEnt = scene->CreateEntity();
					EntityComponent* ent = scene->AddComponent<EntityComponent>(newEnt);
					ent->id = newEnt;
					scene->SetParent(newEnt, selectedEntity);
					break;
				}
				case SceneHierarchyAction::CreateRootEntity: {
					EntID newEnt = scene->CreateEntity();
					EntityComponent* ent = scene->AddComponent<EntityComponent>(newEnt);
					ent->id = newEnt;
					break;
				}
				case SceneHierarchyAction::RemoveEntity: {
					EntID selectedEntity = m_view.GetSelectedEntity();
					scene->RemoveEntity(selectedEntity);
					break;
				}
				default:
					break;
				}
			});

		m_view.SetActionLeftClick(
			[this]() {
				EntID selectedEntity = m_view.GetSelectedEntity();
				Scene* scene = m_context.GetCurrentScene();
				if (scene->HasEntity(selectedEntity)) {
					m_inspectorPanel.SetSelectedEntity(selectedEntity);
				}
			}
		);


	}

}
