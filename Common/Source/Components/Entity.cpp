#include "Entity.hpp"
#include <ImGui.hpp>
#include <string>
namespace MultiStation::Component {

    Entity::Entity(void) : id(0) {
        strncpy_s(name, "Entity", 170);
    }

    void EntityInspectable::DrawElement(void* data)
    {
        Entity* entity =
            static_cast<Entity*>(data);

        if (!entity)
            return;

        if (!ImGui::CollapsingHeader(
            "Entity",
            ImGuiTreeNodeFlags_DefaultOpen))
        {
            return;
        }

        ImGui::PushStyleVar(
            ImGuiStyleVar_ItemSpacing,
            ImVec2(8.0f, 6.0f)
        );

        ImGui::BeginChild(
            "EntityInspector",
            ImVec2(0.0f, 110.0f),
            ImGuiChildFlags_Borders
        );

        if (ImGui::BeginTable(
            "EntityProperties",
            2,
            ImGuiTableFlags_SizingStretchProp))
        {
            ImGui::TableSetupColumn(
                "Property",
                ImGuiTableColumnFlags_WidthFixed,
                70.0f
            );

            ImGui::TableSetupColumn(
                "Value",
                ImGuiTableColumnFlags_WidthStretch
            );


            // =====================================================
            // NAME
            // =====================================================

            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Name");

            ImGui::TableSetColumnIndex(1);

            ImGui::SetNextItemWidth(-FLT_MIN);

            ImGui::InputText(
                "##EntityName",
                entity->name,
                170
            );


            // =====================================================
            // ID
            // =====================================================

            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("ID");

            ImGui::TableSetColumnIndex(1);

            ImGui::TextDisabled(
                "%llu",
                static_cast<unsigned long long>(
                    entity->id
                    )
            );


            // =====================================================
            // UUID
            // =====================================================

            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("UUID");

            ImGui::TableSetColumnIndex(1);

            ImGui::TextDisabled(
                "%llu",
                static_cast<unsigned long long>(
                    entity->handle
                    )
            );


            ImGui::EndTable();
        }

        ImGui::EndChild();

        ImGui::PopStyleVar();
    }



    bool EntitySerializable::Serialize(void* ref, IArchiveWriter* archive) {
        Entity* ent = static_cast<Entity*>(ref);
        if (!ent || !archive) return false;

        archive->BeginObject("EntityComponent");
        archive->WriteField("name", ent->name);
        archive->WriteField("UUID", std::to_string((uint64_t)ent->handle));
        archive->EndObject();

        return true;
    }

    bool EntitySerializable::Deserialize(void* ref, IArchiveReader* archive) {
        Entity* ent = static_cast<Entity*>(ref);
        if (!ent || !archive) return false;

        archive->BeginObject("EntityComponent");
        std::string name;
        archive->ReadField("name", name);
        std::string uuid;
        archive->ReadField("UUID", uuid);
        archive->EndObject();

        strncpy_s(ent->name, name.data(), 100 > name.length() ? name.length() : 100);
        uint64_t number = std::stoull(uuid);
        ent->handle = UUID(number);
        return true;
    }

}

