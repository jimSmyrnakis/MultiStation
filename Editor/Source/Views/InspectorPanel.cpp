#include "InspectorPanel.hpp"

namespace MultiStation {
    static void BeginStyle()
    {
        // Layout
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 6.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 3.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f, 4.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(5.0f, 3.0f));

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

        // Main panel
        ImGui::PushStyleColor(
            ImGuiCol_WindowBg,
            ImVec4(0.24f, 0.25f, 0.27f, 1.0f)
        );

        // Collapse headers
        ImGui::PushStyleColor(
            ImGuiCol_Header,
            ImVec4(0.30f, 0.32f, 0.35f, 1.0f)
        );

        ImGui::PushStyleColor(
            ImGuiCol_HeaderHovered,
            ImVec4(0.38f, 0.43f, 0.50f, 1.0f)
        );

        ImGui::PushStyleColor(
            ImGuiCol_HeaderActive,
            ImVec4(0.32f, 0.40f, 0.50f, 1.0f)
        );

        // Input fields
        ImGui::PushStyleColor(
            ImGuiCol_FrameBg,
            ImVec4(0.31f, 0.32f, 0.35f, 1.0f)
        );

        ImGui::PushStyleColor(
            ImGuiCol_FrameBgHovered,
            ImVec4(0.38f, 0.40f, 0.44f, 1.0f)
        );

        ImGui::PushStyleColor(
            ImGuiCol_FrameBgActive,
            ImVec4(0.34f, 0.40f, 0.48f, 1.0f)
        );

        // Borders / separators
        ImGui::PushStyleColor(
            ImGuiCol_Border,
            ImVec4(0.16f, 0.17f, 0.19f, 1.0f)
        );

        ImGui::PushStyleColor(
            ImGuiCol_Separator,
            ImVec4(0.36f, 0.38f, 0.42f, 1.0f)
        );

        // Text
        ImGui::PushStyleColor(
            ImGuiCol_Text,
            ImVec4(0.90f, 0.91f, 0.93f, 1.0f)
        );

        ImGui::PushStyleColor(
            ImGuiCol_TextDisabled,
            ImVec4(0.58f, 0.60f, 0.64f, 1.0f)
        );

        // Buttons
        ImGui::PushStyleColor(
            ImGuiCol_Button,
            ImVec4(0.29f, 0.31f, 0.34f, 1.0f)
        );

        ImGui::PushStyleColor(
            ImGuiCol_ButtonHovered,
            ImVec4(0.38f, 0.44f, 0.53f, 1.0f)
        );

        ImGui::PushStyleColor(
            ImGuiCol_ButtonActive,
            ImVec4(0.31f, 0.39f, 0.50f, 1.0f)
        );

        // Extra contrast for title/tab areas if visible
        ImGui::PushStyleColor(
            ImGuiCol_TitleBg,
            ImVec4(0.20f, 0.21f, 0.23f, 1.0f)
        );

        ImGui::PushStyleColor(
            ImGuiCol_TitleBgActive,
            ImVec4(0.25f, 0.28f, 0.32f, 1.0f)
        );

        ImGui::PushStyleColor(
            ImGuiCol_Tab,
            ImVec4(0.25f, 0.27f, 0.30f, 1.0f)
        );

        ImGui::PushStyleColor(
            ImGuiCol_TabHovered,
            ImVec4(0.36f, 0.42f, 0.50f, 1.0f)
        );

        ImGui::PushStyleColor(
            ImGuiCol_TabSelected,
            ImVec4(0.31f, 0.36f, 0.43f, 1.0f)
        );
    }

    static void EndStyle()
    {
        ImGui::PopStyleColor(19);
        ImGui::PopStyleVar(7);
    }

	InspectorPanel::InspectorPanel(void) noexcept  : IDrawable() {
		m_SelectedEntity = nullEntity;
		m_context = nullptr;
		Editor::RegisterInspectable<EntityComponent>(s_entityComponentInspector);
	}


	void InspectorPanel::Draw() noexcept
	{
		BeginStyle();

		ImGui::Begin(
			"Inspector",
			nullptr,
			ImGuiWindowFlags_NoCollapse
		);

		DrawElements();
		DrawAddComponentButton();

		ImGui::End();

		EndStyle();
	}

	void InspectorPanel::DrawAddComponentButton(void) {
		if (m_context == nullptr || m_SelectedEntity == nullEntity) {
			return;
		}
		ImGui::Spacing();

		const float buttonWidth = 42.0f;
		const float buttonHeight = 28.0f;

		float availableWidth = ImGui::GetContentRegionAvail().x;
		float offset = (availableWidth - buttonWidth) * 0.5f;

		if (offset > 0.0f)
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);

		if (ImGui::Button("+", ImVec2(buttonWidth, buttonHeight)))
		{
			// TODO: Add Component
		}
	}

	void InspectorPanel::SetEngineContext(EngineContext& ctx) noexcept {
		m_context = &ctx;
	}

	void InspectorPanel::SetSelectedEntity(EntID entity) noexcept {
		m_SelectedEntity = 
			entity;
	}

	EntID InspectorPanel::GetSelectedEntity(void) const noexcept {
		return m_SelectedEntity;
	}

	void InspectorPanel::DrawElements(void) noexcept {
		// if there is no context or no selected entity, we don't draw anything
		if (m_context == nullptr || m_SelectedEntity == nullEntity) {
			return;
		}

		// now take the selected entity and draw its Entity Components
		// 0. Take the scene
		Scene* scene = m_context->GetCurrentScene();

		// 1. take from scene the components ids of this entity 
		std::span<const CompID> ids = scene->GetEntityComponentsIDs(m_SelectedEntity);
		// 2. for each component draw
		IInspectable* inspectable = nullptr;
		void* data = nullptr;
		for (auto id : ids) {
			// 2.1 take the data
			data = scene->GetComponent(id, m_SelectedEntity);
			// 2.2 take obj common id
			ObjID obj_id;
			if (!scene->GetComponentObjectID(id, obj_id))
				continue;
			// 2.3 now take inspectable
			inspectable = Editor::GetInspectable(obj_id);
			// 2.4 draw element
			if (data && inspectable) {

				inspectable->DrawElement(data);


			}
		}
		
	}

	EntityComponentInspector InspectorPanel::s_entityComponentInspector;

	
}
