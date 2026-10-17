#include "EditorRenderViewport.hpp"

namespace MultiStation {

	/**/


    void EditorRenderViewport::Draw(void) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoScrollWithMouse;
        ImGui::Begin("Viewport", nullptr,
            flags);

        viewport = ImGui::GetContentRegionAvail();

        ImGui::Image((void*)(uintptr_t)colorTexture,
            viewport,
            ImVec2(0, 1),
            ImVec2(1, 0)
        );

        ImGui::End();
        ImGui::PopStyleVar();
    }

}
