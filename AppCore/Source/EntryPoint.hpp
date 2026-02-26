#pragma once
#include "App/Application.hpp"



int main(int argc, char** argv) {
	MultiStation::LogInit();

	MultiStation::Application* app = MultiStation::Application::GetInstance();
	MultiStation::InitGraphicsApi(app->GetWindow()->GetSurfaceWidth(),
		app->GetWindow()->GetSurfaceHeight());
	app->Init(new MultiStation::ImguiLayer());
	app->Run();


	return 0;
}
