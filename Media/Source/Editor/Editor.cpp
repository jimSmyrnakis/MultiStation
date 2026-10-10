#include "Editor.hpp"
namespace MultiStation {

	
	EditorLayer& Editor::GetLayer(void) noexcept {
		return s_layer;
	}

	
	void Editor::AddDrawable(IDrawable* drawable) noexcept {
		s_layer.AddDrawable(drawable);
	}

	
	void Editor::RemoveDrawable(IDrawable* drawable) noexcept {
		s_layer.RemoveDrawable(drawable);
	}

	
	bool Editor::HasDrawable(IDrawable* drawable) noexcept {
		return s_layer.HasDrawable(drawable);
	}

	EditorLayer Editor::s_layer;



	IInspectable* Editor::GetInspectable(ObjID id) {
		return s_layer.GetInspectable(id);
	}


	bool Editor::HasInspectable(ObjID id){
		return s_layer.HasInspectable(id);
	}
}
