#pragma once
#include "../ECSRegistry/Registry.hpp"
#include <unordered_map>
/**
 * @author Dimitris Smyrnakis
 * @version 0.1
 * @file SceneGraph.hpp
 * @brief This file contains the class responsible for managing entities hierarchy presentation .
 */
namespace MultiStation{

	/**
	 * @brief Stores an entity's identity and links within a SceneGraph.
	 * @note Node storage is owned by the graph; parent and child pointers are non-owning.
	 */
	struct EntityNode {
		/** @brief Direct child nodes, in the same order as childs_ids. */
		std::vector<EntityNode*> childs;
		/** @brief IDs corresponding to the pointers in childs. */
		std::vector<EntID> childs_ids;
		/** @brief Entity represented by this node, or nullEntity before initialization. */
		EntID id = nullEntity;
		/** @brief Parent node; nullptr for the graph's root. */
		EntityNode* parent = nullptr;

		/**
		 * @brief Compares node IDs without comparing their hierarchy links.
		 * @param node Node to compare with this node.
		 * @return True when both nodes have the same entity ID.
		 */
		bool operator==(EntityNode& node) noexcept ;
		/**
		 * @brief Compares node IDs through a const node.
		 * @param node Node to compare with this node.
		 * @return True when both nodes have the same entity ID.
		 */
		bool operator==(const EntityNode& node) const noexcept ;
	};

	

	/**
	 * @brief Maintains an acyclic parent-child hierarchy of entity IDs.
	 * This class manages hierarchy only; it does not create or destroy registry
	 * entities, manage components, or verify that IDs exist in an ECSRegistry.
	 * @warning Not thread-safe. 
	 * @note Copying and moving are disabled .
	 */
	class SceneGraph {
	public:
		
		/** @brief Creates a graph containing only the rootEntity node, with no parent. */
		SceneGraph(void);

		/** @brief Disables copying of internal hierarchy pointers. */
		SceneGraph(const SceneGraph&) = delete;

		/** @brief Disables moving of the graph. */
		SceneGraph(SceneGraph&&) = delete;

		/** @brief Disables replacing the graph by copying another graph. */
		SceneGraph& operator=(const SceneGraph&) = delete;

		/** @brief Disables replacing the graph by moving another graph. */
		SceneGraph& operator=( SceneGraph&&) = delete;


		/**
		 * @brief Adds an entity as a direct child of rootEntity.
		 * @param entity ID to insert into the hierarchy.
		 * @return True if inserted; false if already present or equal to rootEntity
		 * or nullEntity.
		 */
		bool AddEntity(EntID entity) noexcept;

		/**
		 * @brief Removes an entity and reparents its direct children to its parent.
		 * @param entity ID to remove from the hierarchy.
		 * @return True if removed; false if absent or equal to rootEntity or nullEntity.
		 * @note Descendants are preserved. The registry entity itself is not destroyed.
		 */
		bool RemoveEntity(EntID entity) noexcept;

		/**
		 * @brief Moves an entity and its subtree beneath the requested parent.
		 * @param entity Existing non-root entity to reparent.
		 * @param parent Existing parent ID; rootEntity attaches the entity to the root.
		 * @return True if reparented or already attached to that parent. Returns false
		 * for missing IDs, nullEntity, a root entity being moved, self-parenting,
		 * or a relationship that would create a cycle.
		 */
		bool  SetParent(EntID entity, EntID parent) noexcept;
		
		/**
		 * @brief Retrieves the direct parent of an entity.
		 * @param entity Entity whose parent is requested.
		 * @param[out] out Receives the parent's ID on success; unchanged on failure.
		 * @return True on success; false if absent or equal to rootEntity or nullEntity.
		 */
		bool GetParent(EntID entity , EntID& out) const noexcept;

		/**
		 * @brief Returns a read-only view of an entity's direct child IDs.
		 * @param entity Parent ID; rootEntity retrieves the top-level entities.
		 * @return Child IDs in stored order, or an empty span if there are no children,
		 * the entity is absent, or entity equals nullEntity.
		 * @warning The span borrows graph storage. Reacquire it after hierarchy changes;
		 * it must not be used after the graph is destroyed.
		 */
		std::span<const EntID> GetChilds(EntID entity) const noexcept;



		
	private:

		/**
		 * @brief Walks the proposed parent's ancestors to detect a hierarchy cycle.
		 * @param entity Node that would be moved.
		 * @param parent Proposed parent node.
		 * @return True if the proposed parent is the entity itself or its descendant.
		 * @pre Both arguments point to existing nodes in this graph.
		 */
		bool WouldCreateCycle(EntityNode* entity, EntityNode* parent) noexcept;


	private:
		
		/** @brief Owns all hierarchy nodes, indexed by entity ID, including the root. */
		std::unordered_map<EntID, EntityNode> m_tree;
		/** @brief Non-owning pointer to the permanent root node stored in m_tree. */
		EntityNode* m_root;
		
	};

}
