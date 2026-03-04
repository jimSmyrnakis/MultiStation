#pragma once
#include <ECS.hpp>
#include <Media.hpp>
#include <SceneManager.hpp>
#include <vector>
#include <stdint.h>
#include <stddef.h>
#include "../SystemStack/SystemStack.hpp"
#include "../ImGuiSystem/ImGuiSystem.hpp"
namespace MultiStation{

	class Application {
	public:
		Application(const std::string name) noexcept;
		virtual ~Application(void) noexcept;
	public:
		void Initialize(void) noexcept;

		void Run(void) noexcept;

		void Finalize(void) noexcept;

		void PushSystemLayer(ISystem* system) noexcept;
		void PushSystemOverlay(ISystem* system) noexcept;
		void PopSystemLayer(ISystem* system) noexcept;
		void PopSystemOverlay(ISystem* system) noexcept;


		Scene& GetScene(void) noexcept;
		const Scene& GetScene(void) const noexcept;

		

		Window& GetWindow(void) noexcept;
		const Window& GetWindow(void) const noexcept;

		JobSystem& GetJobSystem(void) noexcept;
		const JobSystem& GetJobSystem(void) const noexcept;
		
		

		bool IsRunning(void) const noexcept;
		void SetRunning(bool isRunning) noexcept;

		void OnEvent(Event& e) noexcept;

		static Application& Get(void) noexcept;

	protected:
		static void SetApplication(Application* app) noexcept;
	
	public:

		virtual void SetUp(void) noexcept;

	protected:

		uint32_t CreatePhase(void) noexcept;
		void BindPhase(uint32_t phase) noexcept;
		void AddSystemOnPhase(ISystem* system) noexcept;
		void RemoveSystemFromPhase(ISystem* system) noexcept;


	

	private:
		enum SystemFlags : uint32_t {
			SYSTEM_ON_LAYER_MANAGER = 0x00000001,
			SYSTEM_ON_UPDATE_MANAGER = 0x00000002
		};
		std::vector<ISystem*> m_systems;
		std::vector<uint32_t> m_systemsFlags;

	private:
		bool OnWindowCloseEvent(WindowCloseEvent& e) noexcept;
		void AddSystem(ISystem* system, uint32_t flags) noexcept;
		void RemoveSystem(ISystem* system, uint32_t flags) noexcept;
		bool IsFlagsField(uint32_t flags) noexcept;
	protected:
		std::string m_name;
		SystemManager m_systemManager;
		std::atomic<bool> m_isRunning;
		Window* m_window;
		SystemStack m_systemStack;
		Input* m_Input;
		ImGuiSystem* m_ImGuiSystem;
		bool isInitialized;
		Scene* m_scene;
	
	private:
		static Application* s_singleton;
	};

}
