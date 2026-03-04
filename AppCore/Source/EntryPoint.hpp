#pragma once
#include <Media.hpp>
#include <ECS.hpp>
#include <Application.hpp>
#include <iostream>
namespace MultiStation {
	extern Application* CreateApplication(void) noexcept;
	extern Application& GetApplication(void) noexcept;
	extern void DestroyApplication(void) noexcept;
}

int main(int argc, char** argv) {
	MultiStation::LogInit();
	MultiStation::Application& app = *MultiStation::CreateApplication();
	
	app.Initialize();
	// Run App SetUp given by the user
	app.SetUp();
	while (app.IsRunning()) {
		app.Run();
	}
	app.Finalize();
	MultiStation::DestroyApplication();

#ifdef _DEBUG
	std::cin.get();
#endif 
	return 0;
}
