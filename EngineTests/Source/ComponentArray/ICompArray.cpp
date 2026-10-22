#include "pch.h"
#include <ECS.hpp>
using namespace MultiStation;
TEST( IComponent_Array , add_component) {

	ComponentArray<int> storage;
	IComponentArray& compArray = storage;
	
	int* added = static_cast<int*>(compArray.AddComponentV(1));
	ASSERT_NE(added, nullptr);
	*added = 42;
	
	EXPECT_EQ( compArray.Count(), 1);
	
	ASSERT_TRUE(static_cast<int*>(compArray.GetComponentV(1)) != nullptr);

	EXPECT_TRUE((*static_cast<int*>(compArray.GetComponentV(1))) == 42);

	EXPECT_TRUE(compArray.HasComponentV(1) == true);

}

TEST( IComponent_Array , add_reduntant_component ){

	ComponentArray<int> storage;
	IComponentArray& compArray = storage;

	int* added = static_cast<int*>(compArray.AddComponentV(1));
	ASSERT_NE(added, nullptr);
	*added = 42;
	int* comp = static_cast<int*>(compArray.AddComponentV(1));

	EXPECT_EQ(compArray.Count(), 1);
	
	ASSERT_EQ(comp, nullptr);
	
	ASSERT_TRUE(static_cast<int*>(compArray.GetComponentV(1)) != nullptr);

	EXPECT_TRUE((*static_cast<int*>(compArray.GetComponentV(1))) == 42);

	EXPECT_TRUE(compArray.HasComponentV(1) == true);
}


TEST(IComponent_Array, replace_component) {

	ComponentArray<int> storage;
	IComponentArray& compArray = storage;

	int* added = static_cast<int*>(compArray.AddComponentV(1));
	ASSERT_NE(added, nullptr);
	*added = 42;
	int* comp = static_cast<int*>(compArray.ReplaceComponentV(1));

	EXPECT_EQ(compArray.Count(), 1);

	ASSERT_NE(comp, nullptr);

	ASSERT_TRUE(static_cast<int*>(compArray.GetComponentV(1)) != nullptr);

	EXPECT_TRUE((*static_cast<int*>(compArray.GetComponentV(1))) == 0);

	EXPECT_TRUE(compArray.HasComponentV(1) == true);
}

TEST(IComponent_Array, replace_non_existing_component) {

	ComponentArray<int> storage;
	IComponentArray& compArray = storage;

	int* added = static_cast<int*>(compArray.AddComponentV(1));
	ASSERT_NE(added, nullptr);
	*added = 42;
	int* comp = static_cast<int*>(compArray.ReplaceComponentV(2));

	EXPECT_EQ(compArray.Count(), 1);

	ASSERT_EQ(comp, nullptr);

	ASSERT_TRUE(static_cast<int*>(compArray.GetComponentV(1)) != nullptr);

	EXPECT_TRUE((*static_cast<int*>(compArray.GetComponentV(1))) == 42);

	EXPECT_TRUE(compArray.HasComponentV(1) == true);

	EXPECT_TRUE(compArray.HasComponentV(2) == false);
}

TEST(IComponent_Array, remove_component) {

	ComponentArray<int> storage;
	IComponentArray& compArray = storage;

	int* added = static_cast<int*>(compArray.AddComponentV(1));
	ASSERT_NE(added, nullptr);
	*added = 42;
	compArray.RemoveComponentV(1);

	EXPECT_EQ(compArray.Count(), 0);

	ASSERT_TRUE(static_cast<int*>(compArray.GetComponentV(1)) == nullptr);

	EXPECT_TRUE(compArray.HasComponentV(1) == false);
}

TEST(IComponent_Array, remove_non_existing_component) {

	ComponentArray<int> storage;
	IComponentArray& compArray = storage;

	int* added = static_cast<int*>(compArray.AddComponentV(1));
	ASSERT_NE(added, nullptr);
	*added = 42;
	compArray.RemoveComponentV(2);

	EXPECT_EQ(compArray.Count(), 1);

	ASSERT_TRUE(static_cast<int*>(compArray.GetComponentV(1)) != nullptr);

	ASSERT_TRUE(static_cast<int*>(compArray.GetComponentV(2)) == nullptr);

	EXPECT_TRUE((*static_cast<int*>(compArray.GetComponentV(1))) == 42);

	EXPECT_TRUE(compArray.HasComponentV(1) == true);
}

TEST(IComponent_Array, remove_from_empty_array_comps) {

	ComponentArray<int> storage;
	IComponentArray& compArray = storage;

	compArray.RemoveComponentV(1);

	EXPECT_EQ(compArray.Count(), 0);

	ASSERT_TRUE(static_cast<int*>(compArray.GetComponentV(1)) == nullptr);

	EXPECT_TRUE(compArray.HasComponentV(1) == false);
}

TEST(IComponent_Array, remove_from_multiple_components) {

	ComponentArray<int> storage;
	IComponentArray& compArray = storage;

	for (EntID id = 1; id < 1001; id++) {
		int* added = static_cast<int*>(compArray.AddComponentV(id));
		ASSERT_NE(added, nullptr);
		*added = id * 2;
	}

	compArray.RemoveComponentV(10);

	EXPECT_EQ(compArray.Count(), 999);

	for (EntID id = 1; id < 1001; id++) {
		if (id != 10 ) {
			ASSERT_TRUE(static_cast<int*>(compArray.GetComponentV(id)) != nullptr);

			EXPECT_TRUE(compArray.HasComponentV(id) == true);

			EXPECT_TRUE((*static_cast<int*>(compArray.GetComponentV(id))) == id * 2);
		}
			
	}

	ASSERT_TRUE( static_cast<int*>(compArray.GetComponentV(10)) == nullptr);

	EXPECT_TRUE(compArray.HasComponentV(10) == false);

	
}

TEST(IComponent_Array, remove_last_from_multiple_components) {

	ComponentArray<int> storage;
	IComponentArray& compArray = storage;

	for (EntID id = 1; id < 1001; id++) {
		int* added = static_cast<int*>(compArray.AddComponentV(id));
		ASSERT_NE(added, nullptr);
		*added = id * 2;
	}

	compArray.RemoveComponentV(1000);

	EXPECT_EQ(compArray.Count(), 999);

	for (EntID id = 1; id < 1001; id++) {
		if (id != 1000) {
			ASSERT_TRUE(static_cast<int*>(compArray.GetComponentV(id)) != nullptr);

			EXPECT_TRUE(compArray.HasComponentV(id) == true);

			EXPECT_TRUE((*static_cast<int*>(compArray.GetComponentV(id))) == id * 2);
		}

	}


	ASSERT_TRUE(static_cast<int*>(compArray.GetComponentV(1000)) == nullptr);

	EXPECT_TRUE(compArray.HasComponentV(1000) == false);

}
