#include "EditorModule.hpp"
namespace MultiStation {





	void EditorModule::Init(EngineContext& ctx)  {
		m_EngineContext = &ctx;
		
		// Register Components to scene
		ctx.GetCurrentScene()->RegisterComponent<EntityComponent>();

		// Register Inspectables to inspector registry in future 


		// Views
		m_HierarchyPanel = new SceneHierarchyPanel();
		m_InspectorPanel = new InspectorPanel();
		
		// Controllers
		m_HierarchyController = new SceneHierarchyController(*m_HierarchyPanel,*m_InspectorPanel , ctx);
		
		// set ups
		m_HierarchyPanel->SetEngineContext(ctx);
		m_InspectorPanel->SetEngineContext(ctx);
		Editor::AddDrawable(m_HierarchyPanel);
		Editor::AddDrawable(m_InspectorPanel);
	}


	void EditorModule::Fini(EngineContext& ctx)  {
		delete m_HierarchyController;
		delete m_HierarchyPanel;
	}

	ISystem& EditorModule::GetSystem(void)  {
		return m_system;
	}
	

}
