#pragma once
#include "App/Application.hpp"



int main(int argc, char** argv) {

	MultiStation::Application* app = MultiStation::Application::GetInstance();

	app->Init();

	app->Entry(app);

	return 0;
}
