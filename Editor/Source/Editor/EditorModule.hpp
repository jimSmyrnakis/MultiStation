#pragma once
#include <ECS.hpp>
#include "../Controllers/SceneHierarchyController.hpp"

#include "../System/EditorSystem.hpp"
namespace MultiStation {
	class EditorModule : public ISystemModule {
	public:
		EditorModule(void)  = default;
		~EditorModule(void)  = default;
		EditorModule(const EditorModule&)  = delete;
		EditorModule& operator=(const EditorModule&)  = delete;
		EditorModule(EditorModule&&)  = delete;
		EditorModule& operator=(EditorModule&&)  = delete;

		/**
		 * 
		 */
		void Init(EngineContext& ctx)  override;


		void Fini(EngineContext& ctx)  override;

		ISystem& GetSystem(void)  override;

	private:
		SceneHierarchyPanel* m_HierarchyPanel;
		SceneHierarchyController* m_HierarchyController;
		InspectorPanel* m_InspectorPanel;
		EngineContext* m_EngineContext;
		EditorSystem m_system;
	};
}	
