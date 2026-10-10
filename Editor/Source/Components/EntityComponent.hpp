#pragma once
#include <ECS.hpp>
#include <Media.hpp>
#include <ImGui.hpp>
#include <string>
namespace MultiStation{

	struct EntityComponent {
		EntID id;
		char name[180];

		EntityComponent(void) : id(0) {
			strncpy_s(name, "Entity" , 170);
		}


	};

    class EntityComponentInspector : public IInspectable
    {
    public:
        void DrawElement(void* data) override
        {
            EntityComponent* entity =
                static_cast<EntityComponent*>(data);

            if (!entity)
                return;

            if (ImGui::CollapsingHeader(
                "Entity",
                ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::PushStyleVar(
                    ImGuiStyleVar_ItemSpacing,
                    ImVec2(8.0f, 6.0f)
                );

                ImGui::BeginChild(
                    "EntityInspector",
                    ImVec2(0.0f, 100.0f),
                    ImGuiChildFlags_Borders
                );

                ImGui::BeginTable(
                    "EntityProperties",
                    2,
                    ImGuiTableFlags_SizingStretchProp
                );

                ImGui::TableSetupColumn(
                    "Property",
                    ImGuiTableColumnFlags_WidthFixed,
                    70.0f
                );

                ImGui::TableSetupColumn(
                    "Value",
                    ImGuiTableColumnFlags_WidthStretch
                );

                // Name
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::AlignTextToFramePadding();
                ImGui::TextUnformatted("Name");

                ImGui::TableSetColumnIndex(1);
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::InputText("##EntityName", entity->name , 170);

                // ID
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::AlignTextToFramePadding();
                ImGui::TextUnformatted("ID");

                ImGui::TableSetColumnIndex(1);

                ImGui::BeginDisabled();
                ImGui::Text(
                    "%llu",
                    static_cast<unsigned long long>(entity->id)
                );
                ImGui::EndDisabled();

                ImGui::EndTable();
                ImGui::EndChild();

                ImGui::PopStyleVar();
            }
        }
    };

}
