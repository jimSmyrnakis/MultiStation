#pragma once
#include "App/Application.hpp"



int main(int argc, char** argv) {
	MultiStation::LogInit();

	MultiStation::Application* app = MultiStation::Application::GetInstance();

	app->Run();
	

	return 0;
}
