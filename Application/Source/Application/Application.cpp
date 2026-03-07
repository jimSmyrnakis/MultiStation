#include "../mspch.h"
#include "Application.hpp"
#include <GLAD.hpp>

namespace MultiStation {
	void Application::SetUp(void) noexcept {}
	FBuffer* fb = nullptr;
	Application::Application(const std::string name, uint32_t threads) noexcept : m_systemManager(threads) {
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

		// poll events and update imgui and game engine events
		m_window->PollEvents();

		// Before all call updates for each phase
		for (uint32_t phase : m_systemManager) {
			m_systemManager.ExecutePhase(phase);
		}

		// Clear previus frame -- TODO use Graphics Library for it
		fb->ClearColorBuffer(0, { 0.4, 0.4, 0.4, 1 });

		

		// Run Render layer from start to end (fifo for game render) 
		for (IMSSystem* system : m_systemStack) {
			system->OnRenderUpdate(0.016f);
		}

		// Now Run ImGui UI Render from start to end (fifo but after the game render)
		m_ImGuiSystem->Begin();
		for (IMSSystem* system : m_systemStack) {
			system->OnEditorUIRender(0.016f);
		}
		m_ImGuiSystem->End();

		// Update the window
		m_window->SwapBuffers();

		// Update the scene
		m_scene.UpdateScene();

		// Update request's for phase based execution systems
		m_systemManager.Update();
	}

	void Application::Finalize(void) noexcept {
		
		

		// Remove systems from existing system / layer managers
		// TODO 
		
		// Free all systems but first detached them
		for (IMSSystem* system : m_systems) {
			system->OnDetach();
			delete system;
		}

	}

	Application::~Application(void) noexcept {
		Window::DestroyWindow(&m_window);
		Input::Destroy();
	}


	


	Scene& Application::GetScene(void) noexcept { return m_scene; }
	const Scene& Application::GetScene(void) const noexcept { return m_scene; }


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
