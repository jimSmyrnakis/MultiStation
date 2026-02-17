#include "Tests/JobSystem/TestJobSystem.hpp"
#include "Tests/SystemManager/SystemManagerTest.hpp"
#include <iostream>
#include <Platform.hpp>
int main(void) {
	MultiStation::LogInit();
	std::cin.get();
	TestJobSystem();
	TestJobSystemNested();
	std::cin.get();
	MS_ENGINE_ERROR("This is an error message with value: %d", 42);
	
	TestSystemManager();
	std::cin.get();
}
