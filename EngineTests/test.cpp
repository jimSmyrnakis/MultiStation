#include "pch.h"
#include <ECS.hpp>
using namespace MultiStation;
TEST( Component_Array , AddComponent_SUCCESFULLY) {

	ComponentArray<int> compArray;
	
	compArray.AddComponent(1, 42);
	
	EXPECT_EQ( compArray.GetComponents().size(), 1);

	EXPECT_TRUE((*compArray.GetComponent(1)) == 42);
}