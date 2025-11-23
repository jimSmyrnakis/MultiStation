#pragma once
#include "../Definitions.h"
namespace MultiStation {




	class Window {

	public:
		Window(void) noexcept;
		~Window(void) noexcept;
		Window(const Window& win) = delete;
		Window(Window&& win) noexcept;
		Window& operator=(const Window& win) = delete;
		Window& operator=(Window&& win) noexcept;
	private :
		Window& MoveOperation(Window* mov) noexcept;

	public:

		void* GetNativeWindow() const ;

		uint16_t GetSurfaceWidth(void) const;
		uint16_t GetSurfaceHeight(void) const ;
		uint16_t GetWidth(void) const;
		uint16_t GetHeight(void) const;
		const std::string& GetName(void) const;

		void SetWidth(uint16_t width);
		void SetHeight(uint16_t height);
		void SetName(const std::string& name);
		



		bool ShouldClose() const ;
		void PollEvents() ;
		void SwapBuffers() ;


	private:
		void* m_NativeWindow;
		uint16_t m_Width;
		uint16_t m_Height;
		mutable std::string m_Name;


	};
}
