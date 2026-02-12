#include "../../JobSystem/JobSystem.hpp"
#include <iostream>
#include <Platform.hpp>
#include <math.h>

/*struct DataIn {
	std::atomic<uint32_t>
};*/
int TestJobSystem(void) {
	unsigned long threadsNum = ceil(2.5f * std::thread::hardware_concurrency());
	MultiStation::JobSystem jobSystem(threadsNum);
	float time = MultiStation::Time::GetTimeInSeconds();
	std::atomic<uint64_t> sink{ 0 };

	auto jobFunc = [](void* data, uint32_t workerID , uint32_t id , uint32_t blockID , uint32_t size) {
		auto* out = static_cast<std::atomic<uint64_t>*>(data);
		uint64_t sum = 0;
		for (uint32_t i = 0; i < 10; ++i) {
			uint64_t j = uint64_t(i) * i;  // no UB
			sum += (j ^ (workerID + 1));
		}
		//std::cout << "Job " << id << " executed by worker " << workerID << " , blockID " << blockID << " , block size " << size << std::endl;
		out->fetch_add(1, std::memory_order_release); // 1 atomic per job
		};
	std::shared_ptr<std::atomic<uint32_t>> counterPtr = std::make_shared<std::atomic<uint32_t>>(0);
	jobSystem.ParallelFor(jobFunc, &sink, 100000, counterPtr);
	jobSystem.WaitFor(*counterPtr);
	std::cout << "total jobs = " << sink.load(std::memory_order_acquire) << " threads num = " <<threadsNum << std::endl;
	std::cout << "All jobs finished, counterPtr = " << counterPtr->load(std::memory_order_relaxed) << std::endl;
	jobSystem.Shutdown();
	float elapsedTime = MultiStation::Time::GetTimeInSeconds() - time;
	std::cout << "Elapsed time: " << elapsedTime << " seconds" << std::endl;
	std::cin.get();
	return 0;
}


