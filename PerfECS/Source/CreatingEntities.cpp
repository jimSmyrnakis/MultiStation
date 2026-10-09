
#include "defs.hpp"
using namespace MultiStation;



int CreatingIntComponentsPerf(uint32_t count, float* duration , ComponentArray<int>& components)
{
	

	float start = Time::GetTimeInSeconds();

	for (uint32_t i = 0; i < count; ++i)
	{
		components.AddComponent(i, 42);
	}

	float end = Time::GetTimeInSeconds();

	*duration = end - start;

	

	return 0;
}

int CreatingIntEntitiesRegistryCompsPerf(uint32_t count, float* duration , Registry& registry) {
	

	registry.RegisterComponent<int>();

	float start = Time::GetTimeInSeconds();

	for (uint32_t i = 0; i < count; ++i)
	{
		EntID entity = registry.CreateEntity();
		registry.AddComponent<int>(entity, 42);
	}

	float end = Time::GetTimeInSeconds();

	*duration = end - start;

	return 0;
}


int CreatingIntEntitiesScenePerf(uint32_t count, float* duration , Scene& scene) {
	

	scene.RegisterComponent<int>();

	float start = Time::GetTimeInSeconds();

	for (uint32_t i = 0; i < count; ++i)
	{
		EntID entity = scene.CreateEntity();
		scene.AddComponent<int>(entity, 42);
	}

	float end = Time::GetTimeInSeconds();

	*duration = end - start;

	return 0;
}
