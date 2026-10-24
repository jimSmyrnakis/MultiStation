#include "pch.h"
#include <ECS.hpp>
#include <vector>
using namespace MultiStation;
TEST(ECS_Registry, add_component) {

	ECSRegistry registry;
	ASSERT_NE(registry.RegisterComponent<int>(), 0);
	EntID entity1 = registry.CreateEntity();
	ASSERT_NE(entity1, nullEntity);
	
	registry.AddComponent<int>(entity1, 42);
	
	EXPECT_EQ( registry.GetComponents<int>().size(), 1);
	
	ASSERT_TRUE(registry.GetComponent<int>(entity1) != nullptr);

	EXPECT_TRUE((*registry.GetComponent<int>(entity1)) == 42);

	EXPECT_TRUE(registry.HasComponent<int>(entity1) == true);

}

TEST(ECS_Registry, add_reduntant_component ){

	ECSRegistry registry;
	ASSERT_NE(registry.RegisterComponent<int>(), 0);
	EntID entity1 = registry.CreateEntity();
	ASSERT_NE(entity1, nullEntity);

	registry.AddComponent<int>(entity1, 42);
	int* comp = registry.AddComponent<int>(entity1, 56);

	EXPECT_EQ(registry.GetComponents<int>().size(), 1);
	
	ASSERT_EQ(comp, nullptr);
	
	ASSERT_TRUE(registry.GetComponent<int>(entity1) != nullptr);

	EXPECT_TRUE((*registry.GetComponent<int>(entity1)) == 42);

	EXPECT_TRUE(registry.HasComponent<int>(entity1) == true);
}


TEST(ECS_Registry, replace_component) {

	ECSRegistry registry;
	ASSERT_NE(registry.RegisterComponent<int>(), 0);
	EntID entity1 = registry.CreateEntity();
	ASSERT_NE(entity1, nullEntity);

	registry.AddComponent<int>(entity1, 42);
	int* comp = registry.ReplaceComponent<int>(entity1, 56);

	EXPECT_EQ(registry.GetComponents<int>().size(), 1);

	ASSERT_NE(comp, nullptr);

	ASSERT_TRUE(registry.GetComponent<int>(entity1) != nullptr);

	EXPECT_TRUE((*registry.GetComponent<int>(entity1)) == 56);

	EXPECT_TRUE(registry.HasComponent<int>(entity1) == true);
}

TEST(ECS_Registry, replace_non_existing_component) {

	ECSRegistry registry;
	ASSERT_NE(registry.RegisterComponent<int>(), 0);
	EntID entity1 = registry.CreateEntity();
	ASSERT_NE(entity1, nullEntity);
	EntID entity2 = registry.CreateEntity();
	ASSERT_NE(entity2, nullEntity);

	registry.AddComponent<int>(entity1, 42);
	int* comp = registry.ReplaceComponent<int>(entity2, 56);

	EXPECT_EQ(registry.GetComponents<int>().size(), 1);

	ASSERT_EQ(comp, nullptr);

	ASSERT_TRUE(registry.GetComponent<int>(entity1) != nullptr);

	EXPECT_TRUE((*registry.GetComponent<int>(entity1)) == 42);

	EXPECT_TRUE(registry.HasComponent<int>(entity1) == true);

	EXPECT_TRUE(registry.HasComponent<int>(entity2) == false);
}

TEST(ECS_Registry, remove_component) {

	ECSRegistry registry;
	ASSERT_NE(registry.RegisterComponent<int>(), 0);
	EntID entity1 = registry.CreateEntity();
	ASSERT_NE(entity1, nullEntity);

	registry.AddComponent<int>(entity1, 42);
	registry.RemoveComponent<int>(entity1);

	EXPECT_EQ(registry.GetComponents<int>().size(), 0);

	ASSERT_TRUE(registry.GetComponent<int>(entity1) == nullptr);

	EXPECT_TRUE(registry.HasComponent<int>(entity1) == false);
}

TEST(ECS_Registry, remove_non_existing_component) {

	ECSRegistry registry;
	ASSERT_NE(registry.RegisterComponent<int>(), 0);
	EntID entity1 = registry.CreateEntity();
	ASSERT_NE(entity1, nullEntity);
	EntID entity2 = registry.CreateEntity();
	ASSERT_NE(entity2, nullEntity);

	registry.AddComponent<int>(entity1, 42);
	registry.RemoveComponent<int>(entity2);

	EXPECT_EQ(registry.GetComponents<int>().size(), 1);

	ASSERT_TRUE(registry.GetComponent<int>(entity1) != nullptr);

	ASSERT_TRUE(registry.GetComponent<int>(entity2) == nullptr);

	EXPECT_TRUE((*registry.GetComponent<int>(entity1)) == 42);

	EXPECT_TRUE(registry.HasComponent<int>(entity1) == true);
}

TEST(ECS_Registry, remove_from_empty_array_comps) {

	ECSRegistry registry;
	ASSERT_NE(registry.RegisterComponent<int>(), 0);
	EntID entity1 = registry.CreateEntity();
	ASSERT_NE(entity1, nullEntity);

	registry.RemoveComponent<int>(entity1);

	EXPECT_EQ(registry.GetComponents<int>().size(), 0);

	ASSERT_TRUE(registry.GetComponent<int>(entity1) == nullptr);

	EXPECT_TRUE(registry.HasComponent<int>(entity1) == false);
}

TEST(ECS_Registry, remove_from_multiple_components) {

	ECSRegistry registry;
	ASSERT_NE(registry.RegisterComponent<int>(), 0);
	std::vector<EntID> entities;

	for (EntID id = 1; id < 1001; id++) {
		EntID entity = registry.CreateEntity();
		ASSERT_NE(entity, nullEntity);
		entities.push_back(entity);
	}

	for (EntID id = 1 ; id < 1001 ; id++)
		registry.AddComponent<int>(entities[id - 1], id * 2);

	registry.RemoveComponent<int>(entities[9]);

	EXPECT_EQ(registry.GetComponents<int>().size(), 999);

	for (EntID id = 1; id < 1001; id++) {
		if (id != 10 ) {
			ASSERT_TRUE(registry.GetComponent<int>(entities[id - 1]) != nullptr);

			EXPECT_TRUE(registry.HasComponent<int>(entities[id - 1]) == true);

			EXPECT_TRUE((*registry.GetComponent<int>(entities[id - 1])) == id * 2);
		}
			
	}

	ASSERT_TRUE( registry.GetComponent<int>(entities[9]) == nullptr);

	EXPECT_TRUE(registry.HasComponent<int>(entities[9]) == false);

	
}

TEST(ECS_Registry, remove_last_from_multiple_components) {

	ECSRegistry registry;
	ASSERT_NE(registry.RegisterComponent<int>(), 0);
	std::vector<EntID> entities;

	for (EntID id = 1; id < 1001; id++) {
		EntID entity = registry.CreateEntity();
		ASSERT_NE(entity, nullEntity);
		entities.push_back(entity);
	}

	for (EntID id = 1; id < 1001; id++)
		registry.AddComponent<int>(entities[id - 1], id * 2);

	registry.RemoveComponent<int>(entities[999]);

	EXPECT_EQ(registry.GetComponents<int>().size(), 999);

	for (EntID id = 1; id < 1001; id++) {
		if (id != 1000) {
			ASSERT_TRUE(registry.GetComponent<int>(entities[id - 1]) != nullptr);

			EXPECT_TRUE(registry.HasComponent<int>(entities[id - 1]) == true);

			EXPECT_TRUE((*registry.GetComponent<int>(entities[id - 1])) == id * 2);
		}

	}


	ASSERT_TRUE(registry.GetComponent<int>(entities[999]) == nullptr);

	EXPECT_TRUE(registry.HasComponent<int>(entities[999]) == false);

}


TEST(ECS_Registry, remove_entity) {

	ECSRegistry registry;
	EntID ent = registry.CreateEntity();

	EXPECT_NE(ent, nullEntity);

	EXPECT_EQ(registry.GetAllEntities().size(), 1);
	EXPECT_EQ(registry.GetAllEntities()[0], ent);
	EXPECT_EQ(registry.HasEntity(ent), true);

	bool res = registry.RemoveEntity(ent);
	EXPECT_EQ(registry.GetAllEntities().size(), 0);
	EXPECT_EQ(res, true);
	EXPECT_EQ(registry.HasEntity(ent), false);

}

TEST(ECS_Registry, remove_non_existing_entity) {

	ECSRegistry registry;
	EntID ent = 100;


	EXPECT_EQ(registry.GetAllEntities().size(), 0);

	bool res = registry.RemoveEntity(ent);
	EXPECT_EQ(registry.GetAllEntities().size(), 0);
	EXPECT_EQ(res, false);
	EXPECT_EQ(registry.HasEntity(ent), false);

}

TEST(ECS_Registry, create_and_add_components_to_entity) {

	ECSRegistry registry;
	EntID ent = registry.CreateEntity();

	registry.RegisterComponent<int>();
	registry.RegisterComponent<float>();

	EXPECT_EQ(registry.HasRegisterComponent<int>(), true);
	EXPECT_EQ(registry.HasRegisterComponent<float>(), true);

	int* ent_int = registry.AddComponent<int>(ent);
	*ent_int = 42;
	
	float* ent_float = registry.AddComponent<float>(ent);
	*ent_float = 4.2f;
	
	EXPECT_EQ(registry.GetComponents<int>().size(), 1);
	EXPECT_EQ(registry.GetComponents<float>().size(), 1);

	ent_int = registry.GetComponent<int>(ent);
	ent_float = registry.GetComponent<float>(ent);

	ASSERT_NE(ent_int, nullptr);
	EXPECT_EQ(*ent_int, 42);
	ASSERT_NE(ent_float, nullptr);
	EXPECT_EQ(*ent_float, 4.2f);

	auto ent_comp_ids = registry.GetSignatureOf(ent);
	EXPECT_EQ(ent_comp_ids.size(), 2);

	bool has_int = false, has_float = false ;
	for (CompID component : ent_comp_ids) {
		has_int = has_int || (GetComponentID<int>() == component);
		has_float = has_float || (GetComponentID<float>() == component);
		
	}

	EXPECT_EQ(has_int && has_float, true);


}