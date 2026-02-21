#pragma once
#include <string>
#include <ECS.hpp>
namespace MultiStation{

	class Application {
	public:

		static void Init(void) noexcept;
		static Application* GetInstance(void) noexcept;

		static void Entry(Application* app) noexcept;

		void SetName(const std::string& name) noexcept;
		const std::string& GetName(void) const noexcept;

		SystemManager* GetSystemManager(void) noexcept;
		const SystemManager* GetSystemManager(void) const noexcept;

		bool IsRunning(void) const noexcept;

		void SetRunning(bool isRunning) noexcept;
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
	};

}
