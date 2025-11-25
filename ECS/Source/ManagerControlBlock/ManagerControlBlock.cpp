#include "ManagerControlBlock.hpp"

namespace MultiStation {

    void ManagerControlBlock::NextVersion() {
        std::lock_guard<std::mutex> guard(m_lock);
        ++m_count;
    }

    uint32_t ManagerControlBlock::GetVersion() const {
        std::lock_guard<std::mutex> guard(m_lock);
        return m_count;
    }

    void ManagerControlBlock::SetState(ManagerState state) {
        std::lock_guard<std::mutex> guard(m_lock);
        m_state = state;
    }

    ManagerState ManagerControlBlock::GetState() const {
        std::lock_guard<std::mutex> guard(m_lock);
        return m_state;
    }

}
