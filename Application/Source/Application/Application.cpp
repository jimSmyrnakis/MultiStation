#include "../mspch.h"
#include "Application.hpp"
#define GLEW_STATIC
#include <GL/glew.h>

namespace MultiStation {
	void Application::SetUp(void) noexcept {}

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

		

		isInitialized = false;
		
		this->SetApplication(this);

	}

	void Application::Initialize(void) noexcept {
		m_isRunning.store(true, std::memory_order_relaxed);

		// Create ImGui System
		m_ImGuiSystem = new(std::nothrow) ImGuiSystem();
		MS_ASSERT(m_ImGuiSystem, "failed allocate memory!");
		
		isInitialized = true;
		// Push it front off layers
		PushSystemOverlay(m_ImGuiSystem);

	}

	void Application::Run(void) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");

		// Before all call updates for each phase
		for (uint32_t phase : m_systemManager) {
			m_systemManager.ExecutePhase(phase);
		}

		// Clear previus frame -- TODO use Graphics Library for it
		glClearColor(0.4, 0.4, 0.4, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// Run event layer from end to start (lifo for events )
		for (auto it = m_systemStack.end(); it != m_systemStack.begin(); ) {
			(*(--it))->OnUpdate(0.016f);
		}

		// Run Render layer from start to end (fifo for game render) 
		for (ISystem* system : m_systemStack) {
			system->OnRenderUpdate(0.016f);
		}

		// Now Run ImGui UI Render from start to end (fifo but after the game render)
		m_ImGuiSystem->Begin();
		for (ISystem* system : m_systemStack) {
			system->OnImGuiRender(0.016f);
		}
		m_ImGuiSystem->End();

		// Update the window
		m_window->OnUpdate();

	}

	void Application::Finalize(void) noexcept {
		
		// Free im gui isystem
		if (m_ImGuiSystem) {
			delete m_ImGuiSystem;
			m_ImGuiSystem = nullptr;
		}

	}

	Application::~Application(void) noexcept {
		Window::DestroyWindow(&m_window);
		Input::Destroy();
	}


	


	ECSManager& Application::GetECS(void) noexcept { return m_systemManager.GetECSManager(); }
	const ECSManager& Application::GetECS(void) const noexcept { return m_systemManager.GetECSManager(); }


	Window& Application::GetWindow(void) noexcept { return *m_window; }
	const Window& Application::GetWindow(void) const noexcept { return *m_window; }

	JobSystem& Application::GetJobSystem(void) noexcept { return m_systemManager.GetJobSystem(); }
	const JobSystem& Application::GetJobSystem(void) const noexcept { return m_systemManager.GetJobSystem(); }


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
