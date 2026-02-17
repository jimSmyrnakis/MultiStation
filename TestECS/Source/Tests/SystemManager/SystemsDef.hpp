#pragma once
#include <ECS.hpp>


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