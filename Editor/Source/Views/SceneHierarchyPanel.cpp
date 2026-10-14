#include "SceneHierarchyPanel.hpp"
namespace MultiStation {


    static void BeginStyle()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 3.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f, 2.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 18.0f);

        ImGui::PushStyleColor(
            ImGuiCol_Header,
            ImVec4(0.18f, 0.22f, 0.28f, 1.0f)
        );

        ImGui::PushStyleColor(
            ImGuiCol_HeaderHovered,
            ImVec4(0.24f, 0.30f, 0.38f, 1.0f)
        );

        ImGui::PushStyleColor(
            ImGuiCol_HeaderActive,
            ImVec4(0.30f, 0.38f, 0.48f, 1.0f)
        );
    }

    static void EndStyle()
    {
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar(3);
    }

    SceneHierarchyPanel::SceneHierarchyPanel(void) noexcept
    {
    }


    void SceneHierarchyPanel::Draw(void) noexcept
    {
        BeginStyle();
        ImGui::Begin("Scene Hierarchy");
        
        Scene* scene = m_EngineContext->GetCurrentScene();

        if (scene != nullptr)
        {
            for (EntID entity : scene->GetChildren(rootEntity))
            {
                DrawEntityNode(*scene, entity);
            }
        }


        // Right click σε κενό χώρο του hierarchy.
        ImGuiPopupFlags popup_flags =
            ImGuiPopupFlags_MouseButtonRight |
            ImGuiPopupFlags_NoOpenOverItems;

        if (ImGui::BeginPopupContextWindow(nullptr, popup_flags))
        {
            if (ImGui::MenuItem("Create Root Entity"))
            {
                m_SelectedEntity = rootEntity;
                m_LastAction =
                    SceneHierarchyAction::CreateRootEntity;

                if (m_ActionRightClick)
                    m_ActionRightClick();
            }

            ImGui::EndPopup();
        }

        ImGui::End();
        EndStyle();
    }


    void SceneHierarchyPanel::SetEngineContext(
        EngineContext& ctx) noexcept
    {
        m_EngineContext = &ctx;
    }


    void SceneHierarchyPanel::DrawEntityNode(
        Scene& scene,
        EntID entity) noexcept
    {
        const auto children = scene.GetChildren(entity);

        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_OpenOnDoubleClick |
            ImGuiTreeNodeFlags_SpanAvailWidth;

        if (children.empty())
            flags |= ImGuiTreeNodeFlags_Leaf;


        // Optional:
        // κρατάμε selected state οπτικά.
        if (m_SelectedEntity == entity)
            flags |= ImGuiTreeNodeFlags_Selected;


        char label[64];

        std::snprintf(
            label,
            sizeof(label),
            "Entity %llu",
            static_cast<unsigned long long>(entity)
        );


        const bool opened =
            ImGui::TreeNodeEx(
                reinterpret_cast<void*>(
                    static_cast<uintptr_t>(entity)
                    ),
                flags,
                "%s",
                label
            );


        // Normal left click -> selection only.
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            m_SelectedEntity = entity;

            
			if (m_ActionLeftClick)
				m_ActionLeftClick();
        }
        // drag source
        Editor::BeginDrag<EntID>(new EntID(entity) );

        EntID* val = Editor::AcceptDrop<EntID>();
        if (m_ActionDropEnt && val) {
            m_SelectedEntity = entity;
            m_ActionDropEnt(val);
        }

        // Right click πάνω στη συγκεκριμένη entity.
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Create Child Entity"))
            {
                m_SelectedEntity = entity;
                m_LastAction =
                    SceneHierarchyAction::CreateChildEntity;

                if (m_ActionRightClick)
                    m_ActionRightClick();
            }


            if (ImGui::MenuItem("Create Root Entity"))
            {
                m_SelectedEntity = rootEntity;
                m_LastAction =
                    SceneHierarchyAction::CreateRootEntity;

                if (m_ActionRightClick)
                    m_ActionRightClick();
            }


            ImGui::Separator();


            if (ImGui::MenuItem("Remove Entity"))
            {
                m_SelectedEntity = entity;
                m_LastAction =
                    SceneHierarchyAction::RemoveEntity;

                if (m_ActionRightClick)
                    m_ActionRightClick();
            }

            ImGui::EndPopup();
        }


        if (opened)
        {
            for (EntID child : children)
            {
                DrawEntityNode(scene, child);
            }

            ImGui::TreePop();
        }
    }


    void SceneHierarchyPanel::SetActionRightClick(
        std::function<void()> callback)
    {
        m_ActionRightClick = std::move(callback);
    }

	void SceneHierarchyPanel::SetActionLeftClick(
		std::function<void()> callback)
	{
		m_ActionLeftClick = std::move(callback);
	}

    EntID SceneHierarchyPanel::GetSelectedEntity() const noexcept
    {
        return m_SelectedEntity;
    }

    void SceneHierarchyPanel::SetActionDropEntity(std::function<void(EntID* entity)> callback) {
        m_ActionDropEnt = callback;
    }

    SceneHierarchyAction
        SceneHierarchyPanel::GetLastAction() const noexcept
    {
        return m_LastAction;
    }

}
