#pragma once
#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <atomic>
#include <mutex>
namespace MultiStation {

    struct DAGNode {
    
        uint32_t id;
        std::atomic<uint32_t> inDegree = 0;
        std::vector<uint32_t> outEdges;
    };

    class DAGraph {
    public:
        uint32_t AddNode(uint32_t id);
        void AddEdge(uint32_t from, uint32_t to);

        std::vector<DAGNode> GetNodes(void) const;

        

    private:
        std::vector<DAGNode> m_nodes;
        std::mutex m_lock;
    };


}
