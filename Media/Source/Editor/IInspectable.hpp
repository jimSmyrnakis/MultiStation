#pragma once

namespace MultiStation{

	class IInspectable {

	public:
		virtual ~IInspectable(void) = default;
		/**
		 * @param data Pointer to the object that will be drawn in the inspector panel.
		 * @brief Draws the inspectable object given in data pointer in the inspector panel.
		 */
		virtual void DrawElement(void* data ) = 0;



	};

}
