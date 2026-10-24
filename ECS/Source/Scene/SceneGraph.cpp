#include "SceneGraph.hpp"
namespace MultiStation {

	SceneGraph::SceneGraph(void) {
		// just make the root
		m_tree[rootEntity].id = rootEntity;
		m_root = &m_tree[rootEntity];
		m_root->parent = nullptr;
	}
	
	bool EntityNode::operator==(EntityNode& node) noexcept {
		return id == node.id;
	}
	bool EntityNode::operator==(const EntityNode& node)  const noexcept{
		return id == node.id;
	}
	bool SceneGraph::WouldCreateCycle(EntityNode* entity, EntityNode* parent) noexcept {
		EntityNode* current = parent;

		while (current && current != m_root ) {
			if (current == entity) {
				return true;
			}

			

			current = current->parent;
		}

		return current == entity;
	}

	bool SceneGraph::AddEntity(EntID entity) noexcept {

		if (entity == nullEntity || entity == rootEntity) {
			return false;
		}
		
		auto result = m_tree.try_emplace(entity);
		if (result.second ) {
			EntityNode* node = &result.first->second;
			node->parent = m_root;
			node->id = entity;
			EntityNode& ref = m_tree[rootEntity];
			ref.childs.push_back(node);
			ref.childs_ids.push_back(node->id);
		}
		return result.second;
	}

	bool SceneGraph::RemoveEntity(EntID entity) noexcept {
		auto it = m_tree.find(entity);
		if ( 
			(it == m_tree.end())   || 
			(entity == rootEntity) || 
			(entity == nullEntity) 
		) {
			return false;
		}

		// find list of childs
		auto& childs = it->second.childs;

		// find parent
		EntityNode* parent = it->second.parent;
		EntityNode& parent_node = *parent;
		
		// for each child
		for (EntityNode* child : childs) {
			// update its parent
			child->parent = parent;
			// and update its parent childs
			parent_node.childs.push_back(child);
			parent_node.childs_ids.push_back(child->id);
		}



		// Remove entity from his parent
		auto ent_it = std::find(parent_node.childs.begin(), parent_node.childs.end(), &it->second);
		size_t ent_it_index = std::distance(parent_node.childs.begin(), ent_it);

		MS_ASSERT(ent_it != parent_node.childs.end(), "This Should not happen !");
			
		parent_node.childs.erase(ent_it);
		parent_node.childs_ids.erase(parent_node.childs_ids.begin() + ent_it_index);

		// remove entity from the unordered map
		m_tree.erase(entity);



		return true;
	}

	bool SceneGraph::SetParent(EntID entity, EntID parent) noexcept {
		auto it_entity = m_tree.find(entity);
		auto it_parent = m_tree.find(parent);

		if (
			entity == parent ||
			entity == rootEntity ||
			entity == nullEntity ||
			parent == nullEntity ||
			it_entity == m_tree.end() ||
			it_parent == m_tree.end() ||
			WouldCreateCycle(&it_entity->second, &it_parent->second)
		){
			return false;
		}

		// check if parent is already the same
		if (&it_parent->second == it_entity->second.parent) {
			return true;
		}




		// Remove entity from old parent
		EntityNode* old_parent = it_entity->second.parent;
		auto ent_child_it = std::find(
			old_parent->childs.begin(), old_parent->childs.end(), &it_entity->second);
		MS_ASSERT(ent_child_it != old_parent->childs.end(), "This Should not happen !");
		size_t index = std::distance(old_parent->childs.begin(), ent_child_it);
		old_parent->childs.erase(ent_child_it);
		old_parent->childs_ids.erase(old_parent->childs_ids.begin() + index);

		

		// Set new parent
		it_entity->second.parent = &it_parent->second;

		// Add to new parent's children
		it_parent->second.childs.push_back(&it_entity->second);
		it_parent->second.childs_ids.push_back(it_entity->second.id);

		return true;
	}

	bool SceneGraph::GetParent(EntID entity , EntID& ref) const noexcept {
		auto it = m_tree.find(entity);
		if (it == m_tree.end() || (entity == nullEntity) || (entity == rootEntity) ) {
			return false;
		}
			
		
		ref = it->second.parent->id;
		return true;
	}

	std::span< const EntID> SceneGraph::GetChilds(EntID entity) const noexcept {
		auto it = m_tree.find(entity);
		if (it == m_tree.end() || (entity == nullEntity)) {
			return {};
		}


		return it->second.childs_ids;
	}
}
