#include "Module.hpp"
#include "Views/EditorRenderViewport.hpp"
namespace MultiStation {
	EditorRenderViewport* view;
	void Renderer3DDSModule::Init(EngineContext& context) {
		m_system = new Renderer3DDS;
		view = new EditorRenderViewport;
		Editor::AddDrawable(view);
	}


	void Renderer3DDSModule::Fini(EngineContext& context) {
		delete m_system;
	}


	ISystem& Renderer3DDSModule::GetSystem(void) {
		return *m_system;
	}

}
