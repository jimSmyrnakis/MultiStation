#include "SceneGraph.hpp"
namespace MultiStation {

	SceneGraph::SceneGraph(void) {
		// just make the root
		m_tree[rootEntity].parent = nullEntity;
		
	}

	bool SceneGraph::WouldCreateCycle(EntID entity, EntID parent) {
		EntID current = parent;

		while (current != rootEntity && current != nullEntity) {
			if (current == entity) {
				return true;
			}

			auto it = m_tree.find(current);

			if (it == m_tree.end()) {
				return false;
			}

			current = it->second.parent;
		}

		return current == entity;
	}

	bool SceneGraph::AddEntity(EntID entity) {

		if (entity == nullEntity || entity == rootEntity) {
			return false;
		}
		
		auto result = m_tree.try_emplace(entity);
		if (result.second ) {
			EntityNode* node = &result.first->second;
			node->parent = rootEntity;
			EntityNode& ref = m_tree[rootEntity];
			ref.childs.push_back(entity);
		}
		return result.second;
	}

	bool SceneGraph::RemoveEntity(EntID entity) {
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
		EntID parent = it->second.parent;
		EntityNode& parent_node = m_tree.find(parent)->second;
		
		// for each child
		for (EntID child : childs) {
			// update its parent
			auto child_it = m_tree.find(child);
			child_it->second.parent = parent;
			// and update its parent childs
			parent_node.childs.push_back(child);
		}



		// Remove entity from his parent
		auto entity_parent_it = std::find(
			parent_node.childs.begin(),
			parent_node.childs.end(),
			entity);
		if (entity_parent_it != parent_node.childs.end()) {
			parent_node.childs.erase(
				entity_parent_it
			);
		}
		

		// remove entity from the unordered map
		m_tree.erase(entity);



		return true;
	}

	bool SceneGraph::SetParent(EntID entity, EntID parent) {
		auto it_entity = m_tree.find(entity);
		auto it_parent = m_tree.find(parent);

		if (
			entity == parent ||
			entity == rootEntity ||
			entity == nullEntity ||
			parent == nullEntity ||
			it_entity == m_tree.end() ||
			it_parent == m_tree.end() ||
			WouldCreateCycle(entity, parent)
		){
			return false;
		}

		// check if parent is already the same
		if (it_entity->second.parent == parent) {
			return true;
		}




		// Remove entity from old parent
		auto it_old_parent = m_tree.find(it_entity->second.parent);

		if (it_old_parent != m_tree.end()) {
			auto& childs = it_old_parent->second.childs;

			auto child_it = std::find(
				childs.begin(),
				childs.end(),
				entity
			);

			if (child_it != childs.end()) {
				childs.erase(child_it);
			}
		}

		// Set new parent
		it_entity->second.parent = parent;

		// Add to new parent's children
		it_parent->second.childs.push_back(entity);

		return true;
	}

	bool SceneGraph::GetParent(EntID entity , EntID& ref) const {
		auto it = m_tree.find(entity);
		if (it == m_tree.end() || (entity == nullEntity)) {
			return false;
		}
			
		
		ref = it->second.parent;
		return true;
	}

	std::span< const EntID> SceneGraph::GetChilds(EntID entity) const  {
		auto it = m_tree.find(entity);
		if (it == m_tree.end() || (entity == nullEntity)) {
			return {};
		}


		return it->second.childs;
	}
}
