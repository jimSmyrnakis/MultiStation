#pragma once
#include "ISystem.hpp"
#include <atomic>
/**
 * @author Dimitris Smyrnakis
 * @file ISystemModule.hpp
 * @brief Here is the definition of the system module virtual methods
 * that should implement .
 * @version 0.1
 */
namespace MultiStation {

	
	/**
	 * @class ISystemModule 
	 * @brief Contains the interface of system module for developers to implement
	 * @warning Exceptions terminate the engine , prefer catch them inside the methods and handle them
	 * there .
	 */
	class ISystemModule {
	public:
		
		virtual ~ISystemModule(void) noexcept = default;


		/**
		 * @brief Virtual method for initiallize the system module , 
		 * things like views on editor registry , controllers for handling events services
		 * for the system to execute and communicate settings and properties 
		 * with the user via editor ui .
		 * 
		 * @param context It has all necessery engine context methods like getting the 
		 * active scene or  loading / saving a new scene  etc.
		 * 
		 */
		virtual void Init(EngineContext& context)noexcept = 0;

		/**
		 * @brief Virtual method for when the system module is unregister from the scene , this is
		 * where operations like editor.unregister , components unregister , assets shared sources 
		 * unregister must happen for the system completly remove it self from the scene 
		 * 
		 */
		virtual void Fini(EngineContext& context)noexcept = 0;

		/**
		 * @return It should return the ISystem implementation of this module  .
		 */
		virtual ISystem& GetSystem(void)noexcept = 0;

	public: // Scene based callbacks

		/**
		 * @brief Prepares module resources for a newly created or loaded scene.
		 * @param[in,out] scene Live scene with its initial data ready for use.
		 * @details Use this to prepare per-scene settings or resources without
		 * making the scene active. The default implementation does nothing.
		 * @note SystemManager notifies initialized modules even when their system
		 * updates are disabled. Notifications are synchronous in registration order.
		 * @warning Run scene callbacks at a safe stage with no jobs accessing the
		 * affected scene. Do not mutate module registration, initialize/finalize
		 * modules, or trigger nested lifecycle dispatch from these callbacks.
		 */
		virtual void OnSceneCreate(Scene& scene) noexcept;

		/**
		 * @brief Applies module state for a scene entering active use.
		 * @param[in,out] scene Live scene selected as active by the caller.
		 * @details Use this to bind scene resources or apply its settings. A loaded
		 * scene can be activated repeatedly, with deactivation between activations.
		 * The default implementation does nothing.
		 * @pre Scene creation/loading is complete. The safe-stage and callback
		 * restrictions documented on OnSceneCreate() apply.
		 */
		virtual void OnSceneActivate(Scene& scene) noexcept;

		/**
		 * @brief Stops using a scene as the active scene without destroying it.
		 * @param[in,out] scene Previously active scene, still valid during the call.
		 * @details Release active-scene bindings here. The scene may remain loaded
		 * and become active again, or be destroyed afterward. No replacement scene
		 * is guaranteed. The default implementation does nothing.
		 * @pre The safe-stage and callback restrictions of OnSceneCreate() apply.
		 */
		virtual void OnSceneDeactivate(Scene& scene) noexcept;

		/**
		 * @brief Releases module resources and references before scene destruction.
		 * @param[in,out] scene Scene that is still alive for the duration of the call.
		 * @details Do not retain references to the scene after destruction. This
		 * notification does not save the scene; settings needed by serialization
		 * must be synchronized before saving. The default implementation does nothing.
		 * @pre If active, the scene has received OnSceneDeactivate() first. The
		 * safe-stage and callback restrictions of OnSceneCreate() apply.
		 */
		virtual void OnSceneDestroy(Scene& scene) noexcept;

	};

	

}
