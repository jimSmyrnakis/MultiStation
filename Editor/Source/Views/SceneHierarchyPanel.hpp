#pragma once
#include <ECS.hpp>
#include <Media.hpp>
#include <Application.hpp>
#include <ImGui.hpp>
namespace MultiStation {
	enum class SceneHierarchyAction
	{
		None,
		CreateRootEntity,
		CreateChildEntity,
		RemoveEntity
	};

	class SceneHierarchyPanel : public IDrawable {
	public:
		SceneHierarchyPanel(void) noexcept ;

		void SetEngineContext(EngineContext& ctx) noexcept;

		void Draw(void) noexcept override;

		void SetActionRightClick(std::function<void()> callback);

		void SetActionLeftClick(std::function<void()> callback);

		void SetActionDropEntity(std::function<void(EntID* entity)> callback);

		EntID GetSelectedEntity() const noexcept;
		
		SceneHierarchyAction GetLastAction() const noexcept;

	private:
		
		void DrawEntityNode(Scene& scene, EntID entity) noexcept;

	private:
		EngineContext* m_EngineContext = nullptr;

		EntID m_SelectedEntity = rootEntity;

		SceneHierarchyAction m_LastAction =
			SceneHierarchyAction::None;

		std::function<void()>				m_ActionRightClick;
		std::function<void()>				m_ActionLeftClick;
		std::function<void(EntID* entity)>	m_ActionDropEnt;
	};
}
