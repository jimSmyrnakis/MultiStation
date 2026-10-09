#include "defs.hpp"
using namespace MultiStation;



int IteratingIntComponentsPerf(ComponentArray<int>& components , uint32_t count, float* duration)
{
	

	float start = Time::GetTimeInSeconds();
	int sum = 0;
	auto comps = components.GetComponents();
	for (const auto& component : comps )
	{
		sum += component;
	}

	float end = Time::GetTimeInSeconds();
	printf("Component array sum : %d", sum);
	*duration = end - start;



	return 0;
}


int IteratingIntEntitiesRegistryCompsPerf(uint32_t count, float* duration, Registry& registry) {


	float start = Time::GetTimeInSeconds();
	int sum = 0;
	auto comps = registry.GetComponents<int>();
	for (const auto& component : comps)
	{
		sum += component;
	}

	float end = Time::GetTimeInSeconds();
	printf("Registry sum : %d", sum);
		* duration = end - start;



	return 0;
}


int IteratingIntEntitiesScenePerf(uint32_t count, float* duration, Scene& scene) {


	float start = Time::GetTimeInSeconds();
	int sum = 0;
	auto comps = scene.GetComponents<int>();
	for (const auto& component : comps)
	{
		sum += component;
	}

	float end = Time::GetTimeInSeconds();
	printf("Scene sum : %d", sum);
	*duration = end - start;



	return 0;
}
