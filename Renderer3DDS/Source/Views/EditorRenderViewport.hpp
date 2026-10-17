#pragma once
#include <ImGui.hpp>
#include <Editor.hpp>
#include "../Globals.hpp"
namespace MultiStation {

	class EditorRenderViewport : public IDrawable {

	public:
		virtual ~EditorRenderViewport(void) = default;

		void Draw(void) override;

	private:

	};

}
