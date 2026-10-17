#include "Camera.hpp"
#include <ImGui.hpp>
namespace MultiStation::Component {

	void Camera3DInspectable::DrawElement(void* data) {
		Camera3D* camera = static_cast<Camera3D*>(data);
		if (!camera)
			return;

		ImGui::PushID(data);
		if (ImGui::CollapsingHeader("Camera 3D", ImGuiTreeNodeFlags_DefaultOpen)) {
			if (ImGui::BeginTable("Camera3DProperties", 2,
				ImGuiTableFlags_SizingStretchProp)) {
				ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 90.0f);
				ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted("FOV (degrees)");
				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1.0f);
				ImGui::DragFloat("##FOV", &camera->fov, 0.1f, 1.0f, 179.0f,
					"%.1f", ImGuiSliderFlags_AlwaysClamp);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted("Near Z");
				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1.0f);
				float nearZ = camera->nearZ;
				if (ImGui::DragFloat("##NearZ", &nearZ, 0.001f, 0.0f, 0.0f, "%.4f")
					&& nearZ > 0.0f && nearZ < camera->farZ) {
					camera->nearZ = nearZ;
				}

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted("Far Z");
				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1.0f);
				float farZ = camera->farZ;
				if (ImGui::DragFloat("##FarZ", &farZ, 1.0f, 0.0f, 0.0f, "%.3f")
					&& farZ > camera->nearZ) {
					camera->farZ = farZ;
				}

				ImGui::EndTable();
			}
		}
		ImGui::PopID();
	}

}
