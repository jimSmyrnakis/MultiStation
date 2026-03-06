
#include "defs.hpp"
using namespace MultiStation;
int CreatingEntitiesPerf(uint32_t count , float* duration){

	ECSManager manager;
	float start = Time::GetTimeInSeconds();
	for (uint32_t i = 0; i < count; i++) {
		manager.CreateEntity(i);
	}
	manager.PollOperations();
	float end = Time::GetTimeInSeconds();
	(*duration) = end - start;

	return 0;

}

int IteratingIntComponentsPerf(uint32_t count, float* duration) {
	ECSManager manager;
	manager.Register<int>();
	printf("Creating %u entities with int components...\n", count);
	for (uint32_t i = 0; i < count; i++) {
		if (i % 10000 == 0) {
			printf("Created %u entities...\n", i);
		}
		manager.CreateEntity(i);
		manager.AddComponent<int>(42); // Adding an int component with value 42
	}
	manager.PollOperations();
	float start = Time::GetTimeInSeconds();
	auto components = manager.GetComponents<int>();
	printf("Components count %d", components.size());
	for (auto& component : components) {
		component++; // Incrementing the int component value
	}
	float end = Time::GetTimeInSeconds();
	(*duration) = end - start;
	return 0;
}
