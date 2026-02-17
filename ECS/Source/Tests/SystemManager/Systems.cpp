#include "SystemsDef.hpp"

static int sharedCounterAC = 0;
static int counterB = 0;

void TestSystemA::OnTick(MultiStation::SystemContext* ctx) {
	MS_ENGINE_INFO("TestSystemA tick with deltaTime = %f", ctx->deltaTime);
	for (int i = 0; i < 1000; i++) {
		sharedCounterAC++;
	}

	/*std::shared_ptr<std::atomic<uint32_t>> counterPtr = std::make_shared<std::atomic<uint32_t>>(0);
	ctx->jobSystem->ParallelFor([](MultiStation::Job job) {

		}, nullptr, 1000, counterPtr);

	ctx->jobSystem->WaitFor(counterPtr);*/
}

void TestSystemC::OnTick(MultiStation::SystemContext* ctx) {
	MS_ENGINE_INFO("TestSystemC tick with deltaTime = %f", ctx->deltaTime);
	for (int i = 0; i < 1000; i++) {
		sharedCounterAC++;
	}

	/*std::shared_ptr<std::atomic<uint32_t>> counterPtr = std::make_shared<std::atomic<uint32_t>>(0);
	ctx->jobSystem->ParallelFor([](MultiStation::Job job) {

		}, nullptr, 1000, counterPtr);

	ctx->jobSystem->WaitFor(counterPtr);*/
}

void TestSystemB::OnTick(MultiStation::SystemContext* ctx) {
	MS_ENGINE_INFO("TestSystemB tick with deltaTime = %f", ctx->deltaTime);
	for (int i = 0; i < 1000; i++) {
		counterB++;
	}
	
	/*std::shared_ptr<std::atomic<uint32_t>> counterPtr = std::make_shared<std::atomic<uint32_t>>(0);
	ctx->jobSystem->ParallelFor([](MultiStation::Job job) {
		
		}, nullptr, 1000, counterPtr);

	ctx->jobSystem->WaitFor(counterPtr);*/
}

void TestSystemD::OnTick(MultiStation::SystemContext* ctx) {
	MS_ENGINE_INFO("TestSystemD tick with deltaTime = %f", ctx->deltaTime);
	MS_ENGINE_INFO("TestSystemD sharedCounterAC = %d , counterB = %d", sharedCounterAC, counterB);

	
}


