#pragma once
#include <string>
#include <ECS.hpp>
#include <Platform.hpp>
#include "../Layers/ImGuiLayer/ImGuiLayer.hpp"
#include <Media.hpp>

namespace MultiStation{

	class Application {
	public:

		void Init(ImguiLayer* imgui_master_layer) noexcept;
		
		static Application* GetInstance(void) noexcept;

		static void OnStart(Application* app) noexcept;
		static void OnUpdate(Application* app) noexcept;
		static void OnLeave(Application* app) noexcept;

		void Run(void) noexcept;

		
		void SetName(const std::string& name) noexcept;
		const std::string& GetName(void) const noexcept;

		SystemManager* GetSystemManager(void) noexcept;
		const SystemManager* GetSystemManager(void) const noexcept;

		Window* GetWindow(void) noexcept;
		const Window* GetWindow(void) const noexcept;

		bool IsRunning(void) const noexcept;

		void SetRunning(bool isRunning) noexcept;

		void OnEvent(Event& e);

		bool OnWindowCloseEvent(WindowCloseEvent& e) noexcept;

		void PushLayer(Layer* layer)noexcept;
		void PushOverlay(Layer* overlay)noexcept;
		void PopLayer(Layer* layer)noexcept;
		void PopOverlay(Layer* overlay)noexcept;

		Input* GetInput(void) noexcept;

		const Input* GetInput(void) const noexcept;
	protected:
		Application(void) noexcept ;
		~Application(void) noexcept ;
		Application(const Application&) noexcept = delete;
		Application& operator=(const Application&) noexcept = delete;
		Application(Application&&) noexcept = delete;
		Application& operator=(Application&&) noexcept = delete;

	protected:
		std::string m_name;
		SystemManager* m_systemManager;
		std::atomic<bool> m_isRunning;
		Window* m_window;
		LayerStack m_layerStack;
		Input* m_Input;
		ImguiLayer* m_imguiLayer;
	};

}
