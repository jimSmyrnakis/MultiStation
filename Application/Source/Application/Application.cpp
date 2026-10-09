#include "../mspch.h"
#include "Application.hpp"
#include <GLAD.hpp>

namespace MultiStation {
	void Application::SetUp(Engine& engine) noexcept {}
	FBuffer* fb = nullptr;
	Application::Application(const std::string name) noexcept {
		m_name = name;
		// creating a window
		WindowProperties props;
		props.Title = name;
		props.IsVSync = true;
		props.Width = 720;
		props.Height = 480;
		m_window = Window::CreateWindow(props);
		MS_ASSERT(m_window, "Failed Create a Window!");
		m_window->SetEventCallBack(BIND_EVENT_FN(Application::OnEvent));
		// Initialize Graphics Context
		MultiStation::InitGraphicsApi(m_window->GetSurfaceWidth(),
			m_window->GetSurfaceHeight());
		
		//Initialize and Get Input
		Input::Init(*m_window);
		m_Input = Input::Get();
		MS_ASSERT(m_Input, "Failed to Create Input!");
		
		fb = new FBuffer(MultiStation::Texture2DResolution(m_window->GetWidth() , m_window->GetHeight()));
		

		isInitialized = false;
		
		this->SetApplication(this);

		

	}

	Engine& Application::GetEngine(void) {
		return m_engine;
	}

	void Application::Initialize(void) noexcept {
		m_isRunning.store(true, std::memory_order_relaxed);

		// Create ImGui System
		m_ImGuiLayer = new(std::nothrow) ImGuiLayer();
		MS_ASSERT(m_ImGuiLayer, "failed allocate memory!");
		
		isInitialized = true;
		// Push it front off layers
		PushOverlay(m_ImGuiLayer);

	}
	static float dt;
	void Application::Run(void) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");

		// poll events and update imgui and game engine events
		m_window->PollEvents();

		// Before all call updates for each engine
		m_engine.OnUpdate(0.016f);

		// Clear previus frame -- TODO use Graphics Library for it
		//fb->ClearColorBuffer(0, { 0.4, 0.4, 0.4, 1 });

		

		// draw ui 
		m_ImGuiLayer->Begin();
		m_layers.OnUIRender(0.016f);
		m_ImGuiLayer->End();

		
		// Update the window
		m_window->SwapBuffers();

		
	}

	void Application::Finalize(void) noexcept {
		
		

		
		

	}

	Application::~Application(void) noexcept {
		Window::DestroyWindow(&m_window);
		Input::Destroy();
	}


	


	

	Window& Application::GetWindow(void) noexcept { return *m_window; }
	const Window& Application::GetWindow(void) const noexcept { return *m_window; }

	

	bool Application::IsRunning(void) const noexcept {
		return m_isRunning.load(std::memory_order_relaxed);
	}

	void Application::SetRunning(bool isRunning) noexcept {
		m_isRunning.store(isRunning, std::memory_order_relaxed);
	}


	Application* Application::s_singleton = nullptr;

	void Application::SetApplication(Application* app) noexcept {
		MS_ASSERT(app, "Null app!!!");
		s_singleton = app;
	}

	Application& Application::Get(void) noexcept {
		MS_ASSERT(s_singleton, "No instance of app");
		return *s_singleton;
	}


}
