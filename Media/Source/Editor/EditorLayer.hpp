#pragma once
#include "IDrawable.hpp"
#include "../Layer/Layer.hpp"
#include "InspectorRegistry.hpp"
#include <ImGui.hpp>
#include <vector>
namespace MultiStation{
	template<typename T>
	static const char* GetDragDropType()
	{
		static std::string id =
			"OBJ_" + std::to_string(GetObjectID<T>());

		return id.c_str();
	}
	class EditorLayer : public Layer {

	public:

		EditorLayer(void) ;
		~EditorLayer() = default;
		
		/**
		 * @param drawable The drawable to add to the list of drawables.
		 * @brief Adds a non existing drawable to the EditorLayer .
		 * If the drawable already exists in the list, it will not be added again.
		 */
		void AddDrawable(IDrawable* drawable);

		/**
		 * @param drawable The drawable to remove from the list of drawables.
		 * @brief Removes a drawable from the EditorLayer.
		 * If the drawable does not exist in the list, it will not be removed.
		 */
		void RemoveDrawable(IDrawable* drawable);

		/**
		 * @param drawable The drawable to check if it exists in the list of drawables.
		 * @brief Checks if a drawable exists in the EditorLayer.
		 * @return true if the drawable exists in the list, false otherwise.
		 */
		bool HasDrawable(IDrawable* drawable);

		/**
		 * @param inspector The inspectable to register in the Editor.
		 * @tparam T The type of the object to register.
		 * @brief Registers an inspectable object in the Editor.
		 */
		template<typename T>
		void RegisterInspectable(IInspectable& inspectable);

		/**
		 * @param id The ID of the object to get the inspectable for.
		 * @return A pointer to the inspectable object for the given ID, or nullptr if it does not exist.
		 * @brief Gets the inspectable object for a given object ID.
		 */
		IInspectable* GetInspectable(ObjID id) ;

		/**
		 * @param id The ID of the object to check if it has an inspectable.
		 * @return true if the object has an inspectable, false otherwise.
		 * @brief Checks if an object has an inspectable registered in the Editor.
		 */
		bool HasInspectable(ObjID id) ;

		/**
		 * @brief Called when the EditorLayer is attached to the LayerStack.
		 * This method is used to initialize resources or set up state.
		 */
		void OnAttach(void) noexcept override;

		/**
		 * @brief Called when the EditorManager is detached from the LayerStack.
		 * This method is used to clean up resources or reset state.
		 */
		void OnDetach(void) noexcept override;

		/**
		 * @param event The event to handle.
		 * @brief Called when an event is dispatched to the EditorLayer.
		 * This method is used to handle events such as input or window events.
		 */
		void OnEvent(Event& event)noexcept override;


		/**
		 * @param dt The delta time since the last frame.
		 * @brief Called when the EditorLayer is rendered.
		 * This method is used to render the UI elements of the EditorLayer.
		 */
		void OnUIRender(float dt)noexcept override;

		/**
		 * @tparam T Is the templated generic type that is translated directly to 
		 * a dragging unique id .
		 * @param[in] ptr The pointer to drag for the UI drag action . 
		 * @brief A generic drag implementation for dragging objects and parsing 
		 * pointer to the message . A common protocol that everybody can agree what
		 * can dragged and dropped where , without the need of each module nows other
		 * modules drag ids to accept . 
		 */
		template<typename T>
		void BeginDrag(T* ptr);

		/**
		 * @tparam T Is the templated generic type that is translated directly to 
		 * a dragging unique id .
		 * @brief A common protocol for someone to let his ui accept a dropped object 
		 * without the need of communicating targets ids for drag and drop actions . 
		 * @returns nullptr if there is no drop happening or if there is a drop then returns 
		 * the dragged object address .
		 */
		template<typename T>
		T* AcceptDrop(void);

	private:
		/**
		 * @brief Draws all the drawables in the EditorLayer.
		 * This method is called during the OnUIRender method to render the UI elements of the drawables.
		 */
		void Draw(void);

	private:
		std::vector<IDrawable*> m_drawables;
		InspectorRegistry m_InspectorRegistry;
	};




	template<typename T>
	void EditorLayer::RegisterInspectable(IInspectable& inspectable) {
		m_InspectorRegistry.RegisterInspectable<T>(inspectable);
	}


	template<typename T>
	void EditorLayer::BeginDrag(T* ptr) {
		if (!ptr) return;

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(
				GetDragDropType<T>(),
				&ptr,
				sizeof(T*)
			);

			ImGui::TextUnformatted(GetDragDropType<T>());

			ImGui::EndDragDropSource();
		}
	}

	


	template<typename T>
	T* EditorLayer::AcceptDrop()
	{
		if (!ImGui::BeginDragDropTarget())
			return nullptr;

		const ImGuiPayload* payload =
			ImGui::AcceptDragDropPayload(
				GetDragDropType<T>()
			);

		T* result = nullptr;

		if (payload && payload->DataSize == sizeof(T*))
		{
			result = *static_cast<T* const*>(payload->Data);
		}

		ImGui::EndDragDropTarget();

		return result;
	}

}
