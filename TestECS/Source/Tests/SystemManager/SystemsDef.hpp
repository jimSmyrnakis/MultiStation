#pragma once
#include <ECS.hpp>

class MyComponent : public MultiStation::IComponent<MyComponent> {
public:
	MyComponent(int data); 
	MyComponent(const MyComponent& cpy) = delete;
	MyComponent& operator=(const MyComponent& cpy) = delete;
	MyComponent(MyComponent&& move) noexcept ;
	MyComponent& operator=(MyComponent&& move) noexcept ;

private:
	int data;
};

class TestSystemA : public MultiStation::ISystem {
	// Called every new iteration
	virtual void OnTick(MultiStation::SystemContext* ctx) ;

};

class TestSystemB : public MultiStation::ISystem {
	// Called every new iteration
	virtual void OnTick(MultiStation::SystemContext* ctx);
};

class TestSystemC : public MultiStation::ISystem {
	// Called every new iteration
	virtual void OnTick(MultiStation::SystemContext* ctx);
};


class TestSystemD : public MultiStation::ISystem {
	// Called every new iteration
	virtual void OnTick(MultiStation::SystemContext* ctx);
};