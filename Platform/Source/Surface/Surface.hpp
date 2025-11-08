#pragma once
#include "../Definitions.hpp"
namespace MultiStation{

	class Surface {

	public:
		Surface(void) {}
		virtual ~Surface(void) {}
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual bool Init(void) = 0;
		virtual void SwapBuffers(void) = 0;
		virtual void Close(void) = 0;


	};
}
