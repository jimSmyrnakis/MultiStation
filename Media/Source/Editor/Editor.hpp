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


	private:
		static EditorLayer s_layer;

	};


	template<typename T>
	void Editor::RegisterInspectable(IInspectable& inspectable) {
		s_layer.RegisterInspectable<T>(inspectable);
	}

}
