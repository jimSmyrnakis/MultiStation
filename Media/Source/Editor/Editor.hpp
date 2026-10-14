#pragma once
#include "EditorLayer.hpp"
namespace MultiStation {

	class Editor {

	public :

		/**
		 * @brief Returns the EditorLayer instance.
		 * @return The EditorLayer instance.
		 */
		static EditorLayer& GetLayer(void) noexcept;

		/**
		 * @param drawable The drawable to add to the list of drawables.
		 * @brief Adds a non existing drawable to the Editor.
		 * If the drawable already exists in the list, it will not be added again.
		 */
		static void AddDrawable(IDrawable* drawable) noexcept;

		/**
		 * @param drawable The drawable to remove from the list of drawables.
		 * @brief Removes a drawable from the Editor.
		 * If the drawable does not exist in the list, it will not be removed.
		 */
		static void RemoveDrawable(IDrawable* drawable) noexcept;

		/**
		 * @param drawable The drawable to check if it exists in the list of drawables.
		 * @brief Checks if a drawable exists in the Editor.
		 * @return true if the drawable exists in the list, false otherwise.
		 */
		static bool HasDrawable(IDrawable* drawable) noexcept;



		/**
		 * @param inspector The inspectable to register in the Editor.
		 * @tparam T The type of the object to register.
		 * @brief Registers an inspectable object in the Editor.
		 */
		template<typename T>
		static void RegisterInspectable(IInspectable& inspectable);

		/**
		 * @param id The ID of the object to get the inspectable for.
		 * @return A pointer to the inspectable object for the given ID, or nullptr if it does not exist.
		 * @brief Gets the inspectable object for a given object ID.
		 */
		static IInspectable* GetInspectable(ObjID id) ;

		/**
		 * @param id The ID of the object to check if it has an inspectable.
		 * @return true if the object has an inspectable, false otherwise.
		 * @brief Checks if an object has an inspectable registered in the Editor.
		 */
		static bool HasInspectable(ObjID id) ;

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
		static void BeginDrag(T* ptr);


		/**
		 * @tparam T Is the templated generic type that is translated directly to
		 * a dragging unique id .
		 * @brief A common protocol for someone to let his ui accept a dropped object
		 * without the need of communicating targets ids for drag and drop actions .
		 * @returns nullptr if there is no drop happening or if there is a drop then returns
		 * the dragged object address .
		 */
		template<typename T>
		static T* AcceptDrop(void);



	private:
		static EditorLayer s_layer;

	};


	template<typename T>
	void Editor::RegisterInspectable(IInspectable& inspectable) {
		s_layer.RegisterInspectable<T>(inspectable);
	}

	template<typename T>
	static void Editor::BeginDrag(T* value) {
		s_layer.BeginDrag<T>(value);
	}

	template<typename T>
	static T* Editor::AcceptDrop(void) {
		return s_layer.AcceptDrop<T>();
	}

}
