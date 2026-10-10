#pragma once

namespace MultiStation
{

	class IDrawable
	{
	public :
		
		/**
		 * @brief virtual method for Draw ImGui UI elements
		 * inside it . Basicly every ImGui operations should be done inside this method.
		 */
		virtual void Draw(void) = 0;


		virtual ~IDrawable() = default;

	};

}
