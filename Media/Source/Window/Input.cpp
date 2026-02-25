#include "Input.hpp"
#include "GLFW/MyGLFWInput.hpp"
namespace MultiStation {

	Input* Input::s_Instance  = nullptr;

	bool Input::IsKeyDown(uint32_t key) noexcept {
		MS_ASSERT(s_Instance, "No Singleton Instance !");
		return s_Instance->IsKeyDownImpl(key);
	}

	
	bool Input::IsKeyUp(uint32_t key) noexcept {
		MS_ASSERT(s_Instance, "No Singleton Instance !");
		return s_Instance->IsKeyUpImpl(key);
	}

	bool  Input::IsKeyPressed(uint32_t key) noexcept {
		MS_ASSERT(s_Instance, "No Singleton Instance !");
		return s_Instance->IsKeyPressedImpl(key);
	}

	bool  Input::IsMouseButtonDown(uint32_t button) noexcept{
		MS_ASSERT(s_Instance, "No Singleton Instance !");
		return s_Instance->IsMouseButtonDownImpl(button);
	}

	bool  Input::IsMouseButtonUp(uint32_t button) noexcept{
		MS_ASSERT(s_Instance, "No Singleton Instance !");
		return s_Instance->IsMouseButtonUpImpl(button);
	}

	float Input::GetMouseX(void) noexcept{
		MS_ASSERT(s_Instance, "No Singleton Instance !");
		return s_Instance->GetMouseXImpl();
	}

	float Input::GetMouseY(void) noexcept{
		MS_ASSERT(s_Instance, "No Singleton Instance !");
		return s_Instance->GetMouseYImpl();
	}

	
	Input* Input::Get(void) noexcept {
		MS_ASSERT(s_Instance, "No Singleton Instance !");
		return s_Instance;
	}

	void Input::Init(const Window& win) noexcept {
		
		static GLFWInput* in = new(std::nothrow) GLFWInput(win);
		MS_ASSERT(in, "No memory for Input Singleton !");
		s_Instance = in;
	}

}
