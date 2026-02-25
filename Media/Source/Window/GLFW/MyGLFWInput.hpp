#pragma once
#include "../Input.hpp"
#include <memory>
namespace MultiStation{

	class GLFWInput : public Input {
	public:

		GLFWInput(const Window& win) noexcept ;
		virtual ~GLFWInput(void) noexcept ;
		GLFWInput(const GLFWInput& other) noexcept = delete;
		GLFWInput(GLFWInput&& other) noexcept ;
		GLFWInput& operator=(const GLFWInput& other) noexcept = delete;
		GLFWInput& operator=(GLFWInput&& other) noexcept ;

		virtual bool IsKeyDownImpl(uint32_t key)				noexcept override;
		virtual bool IsKeyUpImpl(uint32_t key)					noexcept override;
		virtual bool IsKeyPressedImpl(uint32_t key)				noexcept override;

		virtual bool IsMouseButtonDownImpl(uint32_t button)		noexcept override;
		virtual bool IsMouseButtonUpImpl(uint32_t button)		noexcept override;

		virtual float GetMouseXImpl(void)						noexcept override;
		virtual float GetMouseYImpl(void)						noexcept override;
	public:

		void* m_Window;
	};

}
