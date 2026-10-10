#include "EditorLayer.hpp"
#include <Platform.hpp>
namespace MultiStation {

	EditorLayer::EditorLayer(void) : Layer("EditorLayer") {
		MS_ENGINE_INFO("EditorLayer::EditorLayer : EditorLayer created");
	}

	void EditorLayer::AddDrawable(IDrawable* drawable) {
		if (drawable == nullptr) {
			MS_ENGINE_WARN("EditorLayer::AddDrawable : drawable is nullptr");
			return;
		}
		if (HasDrawable(drawable)) {
			MS_ENGINE_WARN("EditorLayer::AddDrawable : drawable already exists");
			return;
		}
		m_drawables.push_back(drawable);
	}

	void EditorLayer::RemoveDrawable(IDrawable* drawable) {
		if (drawable == nullptr) {
			MS_ENGINE_WARN("EditorLayer::RemoveDrawable : drawable is nullptr");
			return;
		}
		auto it = std::find(m_drawables.begin(), m_drawables.end(), drawable);
		if (it != m_drawables.end()) {
			m_drawables.erase(it);
		}
		else {
			MS_ENGINE_WARN("EditorLayer::RemoveDrawable : drawable not found");
		}
	}

	bool EditorLayer::HasDrawable(IDrawable* drawable) {
		if (drawable == nullptr) {
			MS_ENGINE_WARN("EditorLayer::HasDrawable : drawable is nullptr");
			return false;
		}
		auto it = std::find(m_drawables.begin(), m_drawables.end(), drawable);
		return it != m_drawables.end();
	}


	void EditorLayer::Draw(void) {
		for (auto& drawable : m_drawables) {
			if (drawable != nullptr) {
				drawable->Draw();
			}
			else {
				MS_ENGINE_WARN("EditorLayer::Draw : drawable is nullptr");
			}
		}
	}

	void EditorLayer::OnAttach(void) noexcept {

	}

	void EditorLayer::OnDetach(void) noexcept {

	}

	void EditorLayer::OnEvent(Event& event)noexcept {

	}

	void EditorLayer::OnUIRender(float dt)noexcept {
		// Draw all the drawables in the EditorManager.
		Draw();
	}

	
	IInspectable* EditorLayer::GetInspectable(ObjID id)  {
		return m_InspectorRegistry.GetInspectable(id);
	}

	
	bool EditorLayer::HasInspectable(ObjID id)  {
		return m_InspectorRegistry.HasInspectable(id);
	}

}
