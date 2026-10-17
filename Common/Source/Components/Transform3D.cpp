#include "Transform3D.hpp"

namespace MultiStation::Component {

    void Transform3DInspectable::DrawElement(void* data)
    {
        Transform3D* transform =
            static_cast<Transform3D*>(data);

        if (!transform)
            return;

        if (!ImGui::CollapsingHeader(
            "Transform 3D",
            ImGuiTreeNodeFlags_DefaultOpen))
        {
            return;
        }

        ImGui::PushStyleVar(
            ImGuiStyleVar_ItemSpacing,
            ImVec2(8.0f, 6.0f)
        );

        ImGui::BeginChild(
            "Transform3DInspector",
            ImVec2(0.0f, 180.0f),
            ImGuiChildFlags_Borders
        );

        auto DrawVec3 =
            [](const char* id,
                glm::vec3& value,
                float dragSpeed,
                float wheelStep)
            {
                ImGui::PushID(id);

                const ImVec4 axisColors[3] =
                {
                    ImVec4(0.80f, 0.20f, 0.20f, 1.0f), // X
                    ImVec4(0.20f, 0.65f, 0.25f, 1.0f), // Y
                    ImVec4(0.20f, 0.35f, 0.80f, 1.0f)  // Z
                };

                const char* axisNames[3] =
                {
                    "X",
                    "Y",
                    "Z"
                };

                const float availableWidth =
                    ImGui::GetContentRegionAvail().x;

                const float spacing =
                    ImGui::GetStyle().ItemSpacing.x;

                const float axisWidth =
                    (availableWidth - spacing * 2.0f) / 3.0f;

                const float axisButtonSize = 24.0f;

                for (int axis = 0; axis < 3; ++axis)
                {
                    if (axis > 0)
                        ImGui::SameLine();

                    ImGui::PushID(axis);

                    ImGui::BeginGroup();

                    // =================================================
                    // AXIS BUTTON
                    // =================================================

                    const ImVec4 baseColor =
                        axisColors[axis];

                    const ImVec4 hoveredColor =
                        ImVec4(
                            std::min(baseColor.x + 0.10f, 1.0f),
                            std::min(baseColor.y + 0.10f, 1.0f),
                            std::min(baseColor.z + 0.10f, 1.0f),
                            1.0f
                        );

                    const ImVec4 activeColor =
                        ImVec4(
                            std::min(baseColor.x + 0.20f, 1.0f),
                            std::min(baseColor.y + 0.20f, 1.0f),
                            std::min(baseColor.z + 0.20f, 1.0f),
                            1.0f
                        );

                    ImGui::PushStyleColor(
                        ImGuiCol_Button,
                        baseColor
                    );

                    ImGui::PushStyleColor(
                        ImGuiCol_ButtonHovered,
                        hoveredColor
                    );

                    ImGui::PushStyleColor(
                        ImGuiCol_ButtonActive,
                        activeColor
                    );

                    ImGui::Button(
                        axisNames[axis],
                        ImVec2(
                            axisButtonSize,
                            axisButtonSize
                        )
                    );

                    ImGui::PopStyleColor(3);


                    // =================================================
                    // DRAG DIRECTLY ON X / Y / Z BUTTON
                    // =================================================

                    if (ImGui::IsItemActive() &&
                        ImGui::IsMouseDragging(
                            ImGuiMouseButton_Left))
                    {
                        const float mouseDelta =
                            ImGui::GetIO().MouseDelta.x;

                        value[axis] +=
                            mouseDelta * dragSpeed;
                    }


                    // Mouse wheel over axis button
                    if (ImGui::IsItemHovered())
                    {
                        const float wheel =
                            ImGui::GetIO().MouseWheel;

                        if (wheel != 0.0f)
                        {
                            value[axis] +=
                                wheel * wheelStep;
                        }
                    }


                    // =================================================
                    // VALUE
                    // =================================================

                    ImGui::SameLine();

                    const float valueWidth =
                        axisWidth
                        - axisButtonSize
                        - spacing;

                    ImGui::SetNextItemWidth(
                        std::max(
                            valueWidth,
                            35.0f
                        )
                    );

                    ImGui::DragFloat(
                        "##Value",
                        &value[axis],
                        dragSpeed,
                        0.0f,
                        0.0f,
                        "%.3f"
                    );


                    // Mouse wheel over numeric value
                    if (ImGui::IsItemHovered())
                    {
                        const float wheel =
                            ImGui::GetIO().MouseWheel;

                        if (wheel != 0.0f)
                        {
                            value[axis] +=
                                wheel * wheelStep;
                        }
                    }

                    ImGui::EndGroup();

                    ImGui::PopID();
                }

                ImGui::PopID();
            };


        if (ImGui::BeginTable(
            "Transform3DProperties",
            2,
            ImGuiTableFlags_SizingStretchProp))
        {
            ImGui::TableSetupColumn(
                "Property",
                ImGuiTableColumnFlags_WidthFixed,
                75.0f
            );

            ImGui::TableSetupColumn(
                "Value",
                ImGuiTableColumnFlags_WidthStretch
            );


            // =====================================================
            // POSITION
            // =====================================================

            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Position");

            ImGui::TableSetColumnIndex(1);

            DrawVec3(
                "Position",
                transform->position,

                // Drag sensitivity
                0.05f,

                // Mouse wheel step
                0.10f
            );


            // =====================================================
            // ROTATION
            // =====================================================

            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Rotation");

            ImGui::TableSetColumnIndex(1);

            DrawVec3(
                "Rotation",
                transform->rotation,

                // Drag sensitivity
                0.25f,

                // 1 degree per wheel step
                1.0f
            );


            // =====================================================
            // SCALE
            // =====================================================

            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Scale");

            ImGui::TableSetColumnIndex(1);

            DrawVec3(
                "Scale",
                transform->scale,

                // Drag sensitivity
                0.01f,

                // Mouse wheel step
                0.05f
            );


            ImGui::EndTable();
        }

        ImGui::EndChild();

        ImGui::PopStyleVar();
    }

	Transform3D::Transform3D(void) {
        position = { 0,0,0 };
        rotation = { 0,0,0 };
        scale    = { 1,1,1 };
        word_matrix = local_matrix = glm::mat4(1);
	}

}
