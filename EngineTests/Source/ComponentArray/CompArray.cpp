#include "pch.h"
#include <ECS.hpp>
using namespace MultiStation;
TEST( Component_Array , add_component) {

	ComponentArray<int> compArray;
	
	compArray.AddComponent(1, 42);
	
	EXPECT_EQ( compArray.GetComponents().size(), 1);
	
	ASSERT_TRUE(compArray.GetComponent(1) != nullptr);

	EXPECT_TRUE((*compArray.GetComponent(1)) == 42);

	EXPECT_TRUE(compArray.HasEntity(1) == true);

}

TEST( Component_Array , add_reduntant_component ){

	ComponentArray<int> compArray;

	compArray.AddComponent(1, 42);
	int* comp = compArray.AddComponent(1, 56);

	EXPECT_EQ(compArray.GetComponents().size(), 1);
	
	ASSERT_EQ(comp, nullptr);
	
	ASSERT_TRUE(compArray.GetComponent(1) != nullptr);

	EXPECT_TRUE((*compArray.GetComponent(1)) == 42);

	EXPECT_TRUE(compArray.HasEntity(1) == true);
}


TEST(Component_Array, replace_component) {

	ComponentArray<int> compArray;

	compArray.AddComponent(1, 42);
	int* comp = compArray.ReplaceComponent(1, 56);

	EXPECT_EQ(compArray.GetComponents().size(), 1);

	ASSERT_NE(comp, nullptr);

	ASSERT_TRUE(compArray.GetComponent(1) != nullptr);

	EXPECT_TRUE((*compArray.GetComponent(1)) == 56);

	EXPECT_TRUE(compArray.HasEntity(1) == true);
}

TEST(Component_Array, replace_non_existing_component) {

	ComponentArray<int> compArray;

	compArray.AddComponent(1, 42);
	int* comp = compArray.ReplaceComponent(2, 56);

	EXPECT_EQ(compArray.GetComponents().size(), 1);

	ASSERT_EQ(comp, nullptr);

	ASSERT_TRUE(compArray.GetComponent(1) != nullptr);

	EXPECT_TRUE((*compArray.GetComponent(1)) == 42);

	EXPECT_TRUE(compArray.HasEntity(1) == true);

	EXPECT_TRUE(compArray.HasEntity(2) == false);
}

TEST(Component_Array, remove_component) {

	ComponentArray<int> compArray;

	compArray.AddComponent(1, 42);
	compArray.RemoveComponent(1);

	EXPECT_EQ(compArray.GetComponents().size(), 0);

	ASSERT_TRUE(compArray.GetComponent(1) == nullptr);

	EXPECT_TRUE(compArray.HasEntity(1) == false);
}

TEST(Component_Array, remove_non_existing_component) {

	ComponentArray<int> compArray;

	compArray.AddComponent(1, 42);
	compArray.RemoveComponent(2);

	EXPECT_EQ(compArray.GetComponents().size(), 1);

	ASSERT_TRUE(compArray.GetComponent(1) != nullptr);

	ASSERT_TRUE(compArray.GetComponent(2) == nullptr);

	EXPECT_TRUE((*compArray.GetComponent(1)) == 42);

	EXPECT_TRUE(compArray.HasEntity(1) == true);
}

TEST(Component_Array, remove_from_empty_array_comps) {

	ComponentArray<int> compArray;

	compArray.RemoveComponent(1);

	EXPECT_EQ(compArray.GetComponents().size(), 0);

	ASSERT_TRUE(compArray.GetComponent(1) == nullptr);

	EXPECT_TRUE(compArray.HasEntity(1) == false);
}

TEST(Component_Array, remove_from_multiple_components) {

	ComponentArray<int> compArray;

	for (EntID id = 1 ; id < 1001 ; id++)
		compArray.AddComponent(id, id * 2);

	compArray.RemoveComponent(10);

	EXPECT_EQ(compArray.GetComponents().size(), 999);

	for (EntID id = 1; id < 1001; id++) {
		if (id != 10 ) {
			ASSERT_TRUE(compArray.GetComponent(id) != nullptr);

			EXPECT_TRUE(compArray.HasEntity(id) == true);

			EXPECT_TRUE((*compArray.GetComponent(id)) == id * 2);
		}
			
	}

	ASSERT_TRUE( compArray.GetComponent(10) == nullptr);

	EXPECT_TRUE(compArray.HasEntity(10) == false);

	
}

TEST(Component_Array, remove_last_from_multiple_components) {

	ComponentArray<int> compArray;

	for (EntID id = 1; id < 1001; id++)
		compArray.AddComponent(id, id * 2);

	compArray.RemoveComponent(1000);

	EXPECT_EQ(compArray.GetComponents().size(), 999);

	for (EntID id = 1; id < 1001; id++) {
		if (id != 1000) {
			ASSERT_TRUE(compArray.GetComponent(id) != nullptr);

			EXPECT_TRUE(compArray.HasEntity(id) == true);

			EXPECT_TRUE((*compArray.GetComponent(id)) == id * 2);
		}

	}


	ASSERT_TRUE(compArray.GetComponent(1000) == nullptr);

	EXPECT_TRUE(compArray.HasEntity(1000) == false);

}
