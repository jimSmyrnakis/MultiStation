
#include "defs.hpp"
using namespace MultiStation;
int CreatingEntitiesPerf(uint32_t count, float* duration);
int IteratingIntComponentsPerf(uint32_t count, float* duration);
int main(void) {
	float duration = 0;
	CreatingEntitiesPerf(10000, &duration);
	printf("Time taken to create 10000 entities: %f seconds\n", duration);
	IteratingIntComponentsPerf(100000, &duration);
	printf("Time taken to iterate 100000 components: %f seconds\n", duration);
	return 0;
}
