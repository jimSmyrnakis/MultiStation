
#include "defs.hpp"
using namespace MultiStation;
int CreatingEntitiesPerf(uint32_t count, float* duration);
int IteratingIntComponentsPerf(uint32_t count, float* duration);
int CreatingIntComponentsPerf(uint32_t count, float* duration);
int main(void)
{
    constexpr int RUNS = 100;
    constexpr int COMPONENTS = 1000000;

    double totalDuration = 0.0;

    for (int i = 0; i < RUNS; ++i)
    {
        float duration = 0.0f;

        CreatingIntComponentsPerf(COMPONENTS, &duration);

        totalDuration += duration;

       // printf("Run %d: %f seconds\n", i + 1, duration);
    }

    double averageDuration = totalDuration / RUNS;

    printf("\nAverage time for %d components over %d runs: %f seconds\n",
        COMPONENTS, RUNS, averageDuration);

    return 0;
}
