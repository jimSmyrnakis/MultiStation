#pragma once
#include "../Views/SceneHierarchyPanel.hpp"
#include "../Views/InspectorPanel.hpp"
namespace MultiStation{

	class SceneHierarchyController {

	public:

		SceneHierarchyController(
			SceneHierarchyPanel& view,
			InspectorPanel& inspectorPanel,
			EngineContext& context) noexcept;



	private:
		SceneHierarchyPanel& m_view;
		InspectorPanel& m_inspectorPanel;
		EngineContext& m_context;
	};
}
