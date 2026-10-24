#include "Module.hpp"
#include "Views/EditorRenderViewport.hpp"
namespace MultiStation {
	EditorRenderViewport* view;
	void Renderer3DDSModule::Init(EngineContext& context) noexcept {
		m_system = new Renderer3DDS(context);
		view = new EditorRenderViewport;
		Editor::AddDrawable(view);
	}


	void Renderer3DDSModule::Fini(EngineContext& context) noexcept {
		delete m_system;
	}


	ISystem& Renderer3DDSModule::GetSystem(void) noexcept {
		return *m_system;
	}

}
