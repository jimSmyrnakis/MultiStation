#include "pch.h"
#include <ECS.hpp>
#include <vector>
using namespace MultiStation;
TEST(ECS_Registry_Non_Templated, add_component) {

	ECSRegistry registry;
	CompID component = registry.RegisterComponent<int>();
	ASSERT_NE(component, 0);
	EntID entity1 = registry.CreateEntity();
	ASSERT_NE(entity1, nullEntity);
	
	int* added = static_cast<int*>(registry.AddComponent(entity1, component));
	ASSERT_NE(added, nullptr);
	*added = 42;
	
	EXPECT_EQ( registry.Count(component), 1);
	
	ASSERT_TRUE(static_cast<int*>(registry.GetComponent(component, entity1)) != nullptr);

	EXPECT_TRUE((*static_cast<int*>(registry.GetComponent(component, entity1))) == 42);

	EXPECT_TRUE(registry.HasComponent(entity1, component) == true);

}

TEST(ECS_Registry_Non_Templated, add_reduntant_component ){

	ECSRegistry registry;
	CompID component = registry.RegisterComponent<int>();
	ASSERT_NE(component, 0);
	EntID entity1 = registry.CreateEntity();
	ASSERT_NE(entity1, nullEntity);

	int* added = static_cast<int*>(registry.AddComponent(entity1, component));
	ASSERT_NE(added, nullptr);
	*added = 42;
	int* comp = static_cast<int*>(registry.AddComponent(entity1, component));

	EXPECT_EQ(registry.Count(component), 1);
	
	ASSERT_EQ(comp, nullptr);
	
	ASSERT_TRUE(static_cast<int*>(registry.GetComponent(component, entity1)) != nullptr);

	EXPECT_TRUE((*static_cast<int*>(registry.GetComponent(component, entity1))) == 42);

	EXPECT_TRUE(registry.HasComponent(entity1, component) == true);
}


TEST(ECS_Registry_Non_Templated, replace_component) {

	ECSRegistry registry;
	CompID component = registry.RegisterComponent<int>();
	ASSERT_NE(component, 0);
	EntID entity1 = registry.CreateEntity();
	ASSERT_NE(entity1, nullEntity);

	int* added = static_cast<int*>(registry.AddComponent(entity1, component));
	ASSERT_NE(added, nullptr);
	*added = 42;
	int* comp = static_cast<int*>(registry.ReplaceComponent(entity1, component));

	EXPECT_EQ(registry.Count(component), 1);

	ASSERT_NE(comp, nullptr);

	ASSERT_TRUE(static_cast<int*>(registry.GetComponent(component, entity1)) != nullptr);

	EXPECT_TRUE((*static_cast<int*>(registry.GetComponent(component, entity1))) == 0);

	EXPECT_TRUE(registry.HasComponent(entity1, component) == true);
}

TEST(ECS_Registry_Non_Templated, replace_non_existing_component) {

	ECSRegistry registry;
	CompID component = registry.RegisterComponent<int>();
	ASSERT_NE(component, 0);
	EntID entity1 = registry.CreateEntity();
	ASSERT_NE(entity1, nullEntity);
	EntID entity2 = registry.CreateEntity();
	ASSERT_NE(entity2, nullEntity);

	int* added = static_cast<int*>(registry.AddComponent(entity1, component));
	ASSERT_NE(added, nullptr);
	*added = 42;
	int* comp = static_cast<int*>(registry.ReplaceComponent(entity2, component));

	EXPECT_EQ(registry.Count(component), 1);

	ASSERT_EQ(comp, nullptr);

	ASSERT_TRUE(static_cast<int*>(registry.GetComponent(component, entity1)) != nullptr);

	EXPECT_TRUE((*static_cast<int*>(registry.GetComponent(component, entity1))) == 42);

	EXPECT_TRUE(registry.HasComponent(entity1, component) == true);

	EXPECT_TRUE(registry.HasComponent(entity2, component) == false);
}

TEST(ECS_Registry_Non_Templated, remove_component) {

	ECSRegistry registry;
	CompID component = registry.RegisterComponent<int>();
	ASSERT_NE(component, 0);
	EntID entity1 = registry.CreateEntity();
	ASSERT_NE(entity1, nullEntity);

	int* added = static_cast<int*>(registry.AddComponent(entity1, component));
	ASSERT_NE(added, nullptr);
	*added = 42;
	registry.RemoveComponent(entity1, component);

	EXPECT_EQ(registry.Count(component), 0);

	ASSERT_TRUE(static_cast<int*>(registry.GetComponent(component, entity1)) == nullptr);

	EXPECT_TRUE(registry.HasComponent(entity1, component) == false);
}

TEST(ECS_Registry_Non_Templated, remove_non_existing_component) {

	ECSRegistry registry;
	CompID component = registry.RegisterComponent<int>();
	ASSERT_NE(component, 0);
	EntID entity1 = registry.CreateEntity();
	ASSERT_NE(entity1, nullEntity);
	EntID entity2 = registry.CreateEntity();
	ASSERT_NE(entity2, nullEntity);

	int* added = static_cast<int*>(registry.AddComponent(entity1, component));
	ASSERT_NE(added, nullptr);
	*added = 42;
	registry.RemoveComponent(entity2, component);

	EXPECT_EQ(registry.Count(component), 1);

	ASSERT_TRUE(static_cast<int*>(registry.GetComponent(component, entity1)) != nullptr);

	ASSERT_TRUE(static_cast<int*>(registry.GetComponent(component, entity2)) == nullptr);

	EXPECT_TRUE((*static_cast<int*>(registry.GetComponent(component, entity1))) == 42);

	EXPECT_TRUE(registry.HasComponent(entity1, component) == true);
}

TEST(ECS_Registry_Non_Templated, remove_from_empty_array_comps) {

	ECSRegistry registry;
	CompID component = registry.RegisterComponent<int>();
	ASSERT_NE(component, 0);
	EntID entity1 = registry.CreateEntity();
	ASSERT_NE(entity1, nullEntity);

	registry.RemoveComponent(entity1, component);

	EXPECT_EQ(registry.Count(component), 0);

	ASSERT_TRUE(static_cast<int*>(registry.GetComponent(component, entity1)) == nullptr);

	EXPECT_TRUE(registry.HasComponent(entity1, component) == false);
}

TEST(ECS_Registry_Non_Templated, remove_from_multiple_components) {

	ECSRegistry registry;
	CompID component = registry.RegisterComponent<int>();
	ASSERT_NE(component, 0);
	std::vector<EntID> entities;

	for (EntID id = 1; id < 1001; id++) {
		EntID entity = registry.CreateEntity();
		ASSERT_NE(entity, nullEntity);
		entities.push_back(entity);
	}

	for (EntID id = 1; id < 1001; id++) {
		int* added = static_cast<int*>(registry.AddComponent(entities[id - 1], component));
		ASSERT_NE(added, nullptr);
		*added = id * 2;
	}

	registry.RemoveComponent(entities[9], component);

	EXPECT_EQ(registry.Count(component), 999);

	for (EntID id = 1; id < 1001; id++) {
		if (id != 10 ) {
			ASSERT_TRUE(static_cast<int*>(registry.GetComponent(component, entities[id - 1])) != nullptr);

			EXPECT_TRUE(registry.HasComponent(entities[id - 1], component) == true);

			EXPECT_TRUE((*static_cast<int*>(registry.GetComponent(component, entities[id - 1]))) == id * 2);
		}
			
	}

	ASSERT_TRUE( static_cast<int*>(registry.GetComponent(component, entities[9])) == nullptr);

	EXPECT_TRUE(registry.HasComponent(entities[9], component) == false);

	
}

TEST(ECS_Registry_Non_Templated, remove_last_from_multiple_components) {

	ECSRegistry registry;
	CompID component = registry.RegisterComponent<int>();
	ASSERT_NE(component, 0);
	std::vector<EntID> entities;

	for (EntID id = 1; id < 1001; id++) {
		EntID entity = registry.CreateEntity();
		ASSERT_NE(entity, nullEntity);
		entities.push_back(entity);
	}

	for (EntID id = 1; id < 1001; id++) {
		int* added = static_cast<int*>(registry.AddComponent(entities[id - 1], component));
		ASSERT_NE(added, nullptr);
		*added = id * 2;
	}

	registry.RemoveComponent(entities[999], component);

	EXPECT_EQ(registry.Count(component), 999);

	for (EntID id = 1; id < 1001; id++) {
		if (id != 1000) {
			ASSERT_TRUE(static_cast<int*>(registry.GetComponent(component, entities[id - 1])) != nullptr);

			EXPECT_TRUE(registry.HasComponent(entities[id - 1], component) == true);

			EXPECT_TRUE((*static_cast<int*>(registry.GetComponent(component, entities[id - 1]))) == id * 2);
		}

	}


	ASSERT_TRUE(static_cast<int*>(registry.GetComponent(component, entities[999])) == nullptr);

	EXPECT_TRUE(registry.HasComponent(entities[999], component) == false);

}

