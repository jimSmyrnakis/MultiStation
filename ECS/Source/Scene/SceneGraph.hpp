#pragma once
#include "../Registry/Registry.hpp"
#include <unordered_map>
namespace MultiStation{

	struct EntityNode {
		std::vector<EntID> childs;
		EntID parent;
	};

	class SceneGraph {
	public:
		
		SceneGraph(void);


		bool AddEntity(EntID entity);

		bool RemoveEntity(EntID entity);

		bool  SetParent(EntID entity, EntID parent);
		
		bool GetParent(EntID entity , EntID& out) const;

		std::span<const EntID> GetChilds(EntID entity) const;

		
	private:

		bool WouldCreateCycle(EntID parent, EntID entity);


	private:
		
		std::unordered_map<EntID, EntityNode> m_tree;

		
	};

}
