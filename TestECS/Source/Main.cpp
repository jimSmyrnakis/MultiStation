#include "Tests/JobSystem/TestJobSystem.hpp"
#include "Tests/SystemManager/SystemManagerTest.hpp"
#include <iostream>
#include <Platform.hpp>
int main(void) {
	
	MultiStation::LogInit();
	MS_ENGINE_ERROR("This is an error message with value: %d", 42);
	MS_ASSERT(true, "This is an assert message with value: %d", 42);
	std::cin.get();
	TestSystemManager();
	std::cin.get();
	TestJobSystem();
	TestJobSystemNested();
	std::cin.get();
	
	
	
}
