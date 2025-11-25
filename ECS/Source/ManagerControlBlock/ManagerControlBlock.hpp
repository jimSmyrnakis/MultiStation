#pragma once
#include <stdint.h>
#include <stddef.h>
#include <mutex>
namespace MultiStation{

    enum class ManagerState {
        UPDATE,
        DESTROY
    };

    class ManagerControlBlock {
    public:
        ManagerControlBlock() noexcept = default;
        ~ManagerControlBlock() = default;

        ManagerControlBlock(const ManagerControlBlock&) = delete;
        ManagerControlBlock& operator=(const ManagerControlBlock&) = delete;

        ManagerControlBlock(ManagerControlBlock&&) = delete;
        ManagerControlBlock& operator=(ManagerControlBlock&&) = delete;

        void NextVersion();

        uint32_t GetVersion() const;

        void SetState(ManagerState state);

        ManagerState GetState() const;

    private:
        mutable std::mutex m_lock;
        uint32_t m_count = 0;
        ManagerState m_state = ManagerState::UPDATE;
    };

}
