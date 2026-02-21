#include "SystemsDef.hpp"

static int sharedCounterAC = 0;
static int counterB = 0;

MyComponent::MyComponent(int data) : data(data) {
	MS_ENGINE_INFO("MyComponent created with data = %d", data);
}

MyComponent::MyComponent(MyComponent&& move) noexcept {
	move.data = 0; // just for testing move semantics
	this->data = move.data;
}
MyComponent& MyComponent::operator=(MyComponent&& move) noexcept {
	return *this;
}

void TestSystemA::OnTick(MultiStation::SystemContext* ctx) {
	MS_ENGINE_INFO("TestSystemA tick with deltaTime = %f", ctx->deltaTime);
	for (int i = 0; i < 1000; i++) {
		sharedCounterAC++;
	}
	//ctx->ecs->AddComponent<MyComponent>(0 , 42);
}

void TestSystemC::OnTick(MultiStation::SystemContext* ctx) {
	MS_ENGINE_INFO("TestSystemC tick with deltaTime = %f", ctx->deltaTime);
	for (int i = 0; i < 1000; i++) {
		sharedCounterAC++;
	}

}

void TestSystemB::OnTick(MultiStation::SystemContext* ctx) {
	MS_ENGINE_INFO("TestSystemB tick with deltaTime = %f", ctx->deltaTime);
	for (int i = 0; i < 1000; i++) {
		counterB++;
	}
	
}

void TestSystemD::OnTick(MultiStation::SystemContext* ctx) {
	MS_ENGINE_INFO("TestSystemD tick with deltaTime = %f", ctx->deltaTime);
	MS_ENGINE_INFO("TestSystemD sharedCounterAC = %d , counterB = %d", sharedCounterAC, counterB);

	
}


