#pragma once

#include <Media.hpp>
#include <ECS.hpp>
#include <vector>
#include <stdint.h>
#include <stddef.h>
#include "../ImGuiLayer/ImGuiLayer.hpp"
namespace MultiStation{

	class Application {
	public:
		Application(const std::string name ) noexcept;
		virtual ~Application(void) noexcept;
	public:
		void Initialize(void) noexcept;

		void Run(void) noexcept;

		void Finalize(void) noexcept;

		void PushLayer(Layer* layer) noexcept;
		void PushOverlay(Layer* layer) noexcept;
		void PopLayer(Layer* layer) noexcept;
		void PopOverlay(Layer* layer) noexcept;

		
		Engine& GetEngine(void);
		

		Window& GetWindow(void) noexcept;
		const Window& GetWindow(void) const noexcept;

		
		
		

		bool IsRunning(void) const noexcept;
		void SetRunning(bool isRunning) noexcept;

		void OnEvent(Event& e) noexcept;

		static Application& Get(void) noexcept;

	protected:
		static void SetApplication(Application* app) noexcept;
	
	public:

		virtual void SetUp(Engine& engine) noexcept;

	protected:

		


	

	private:
		
		LayerStack m_layers;
		Engine m_engine;

	private:
		bool OnWindowCloseEvent(WindowCloseEvent& e) noexcept;
		
	protected:
		std::string m_name;
		
		std::atomic<bool> m_isRunning;
		Window* m_window;
		Input* m_Input;

		ImGuiLayer* m_ImGuiLayer;
		
		bool isInitialized;
		

	
	private:
		static Application* s_singleton;
	};

}
