#pragma once
#include "../Definitions.hpp"
namespace MultiStation {




	class Window {

	public:
		Window(void);
		~Window(void);
		void* GetNativeWindow() const ;
		uint32_t GetSurfaceWidth() const;
		uint32_t GetSurfaceHeight() const ;
		uint32_t GetWidth() const;
		uint32_t GetHeight() const;

		bool ShouldClose() const ;
		void PollEvents() ;
		void SwapBuffers() ;

	private:
		void* m_NativeWindow;
		uint32_t m_Width;
		uint32_t m_Height;


	};
}
