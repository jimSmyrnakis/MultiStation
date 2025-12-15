#include "DAGraph.hpp"
#include <assert.h>
namespace MultiStation {

    
    uint32_t DAGraph::AddNode(uint32_t id) {
        std::unique_lock doLock(m_lock);
        DAGNode node;
        node.id = id;
        m_nodes.push_back(node);
        return static_cast<uint32_t>(m_nodes.size() - 1);
    }

    void DAGraph::AddEdge(uint32_t from, uint32_t to) {
        std::unique_lock doLock(m_lock);
        assert(from < m_nodes.size());
        assert(to < m_nodes.size());
        m_nodes[from].outEdges.push_back(to);
        m_nodes[to].inDegree.fetch_add(1);

    }

    std::vector<DAGNode> DAGraph::GetNodes(void) const {
        return m_nodes;
    }


}
