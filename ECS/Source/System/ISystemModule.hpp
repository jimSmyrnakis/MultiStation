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

	};

	

}
