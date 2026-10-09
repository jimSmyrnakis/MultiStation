
#include "defs.hpp"
using namespace MultiStation;
int CreatingIntComponentsPerf(uint32_t count, float* duration, ComponentArray<int>& components);
int CreatingIntEntitiesRegistryCompsPerf(uint32_t count, float* duration, Registry& registry);
int CreatingIntEntitiesScenePerf(uint32_t count, float* duration, Scene& scene);
int IteratingIntComponentsPerf(ComponentArray<int>& components, uint32_t count, float* duration);
int IteratingIntEntitiesRegistryCompsPerf(uint32_t count, float* duration, Registry& registry);
int IteratingIntEntitiesScenePerf(uint32_t count, float* duration, Scene& scene);
int main(void)
{
    ComponentArray<int> comparray;
    Registry registry;
    EngineContext ctx;
	FileReadStream readStream("test.txt");
	FileWriteStream writeStream("test.txt");
	std::shared_ptr<IArchiveReader> reader = std::make_shared<TextArchiveReader>( &readStream );
	std::shared_ptr<IArchiveWriter> writer = std::make_shared<TextArchiveWriter>( &writeStream);
	SerializationRegistry serializeRegistry;
    Scene scene(ctx, serializeRegistry);
    constexpr int RUNS = 1;
    constexpr int COMPONENTS = 100000;

    double totalDuration = 0.0;

    for (int i = 0; i < RUNS; ++i)
    {
        float duration = 0.0f;

        CreatingIntComponentsPerf(COMPONENTS, &duration , comparray);

        totalDuration += duration;

       // printf("Run %d: %f seconds\n", i + 1, duration);
    }

    double averageDuration = totalDuration / RUNS;

    printf("\n(ComponentArray) : Average time for %d components over %d runs: %f seconds\n",
        COMPONENTS, RUNS, averageDuration);



    for (int i = 0; i < RUNS; ++i)
    {
        float duration = 0.0f;

        CreatingIntEntitiesRegistryCompsPerf(COMPONENTS, &duration , registry);

        totalDuration += duration;

        // printf("Run %d: %f seconds\n", i + 1, duration);
    }

    averageDuration = totalDuration / RUNS;

    printf("\n(Registry) : Average time for %d components over %d runs: %f seconds\n",
        COMPONENTS, RUNS, averageDuration);

    for (int i = 0; i < RUNS; ++i)
    {
        float duration = 0.0f;

        CreatingIntEntitiesScenePerf(COMPONENTS, &duration , scene);

        totalDuration += duration;

        // printf("Run %d: %f seconds\n", i + 1, duration);
    }

    averageDuration = totalDuration / RUNS;

    printf("\n(Scene) : Average time for %d components over %d runs: %f seconds\n",
        COMPONENTS, RUNS, averageDuration);


















    totalDuration = 0;

    for (int i = 0; i < RUNS; ++i)
    {
        float duration = 0.0f;

        IteratingIntComponentsPerf(comparray , COMPONENTS, &duration);

        totalDuration += duration;

        
    }

    averageDuration = totalDuration / RUNS;

    printf("\n(Component array Iteration) : Average time for %d components over %d runs: %f seconds\n",
        COMPONENTS, RUNS, averageDuration);



    totalDuration = 0;
    for (int i = 0; i < RUNS; ++i)
    {
        float duration = 0.0f;

        IteratingIntEntitiesRegistryCompsPerf( COMPONENTS, &duration , registry);

        totalDuration += duration;


    }

    averageDuration = totalDuration / RUNS;

    printf("\n(Registry Iteration) : Average time for %d components over %d runs: %f seconds\n",
        COMPONENTS, RUNS, averageDuration);


    totalDuration = 0;
    for (int i = 0; i < RUNS; ++i)
    {
        float duration = 0.0f;

        IteratingIntEntitiesScenePerf( COMPONENTS, &duration , scene);

        totalDuration += duration;


    }

    averageDuration = totalDuration / RUNS;

    printf("\n(Scene Iteration) : Average time for %d components over %d runs: %f seconds\n",
        COMPONENTS, RUNS, averageDuration);
    
    return 0;
}
