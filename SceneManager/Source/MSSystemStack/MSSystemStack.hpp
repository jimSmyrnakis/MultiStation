#pragma once
#include <ECS.hpp>
#include "../Interfaces/IMSSystem.hpp"
#include <vector>
#include <stdint.h>
#include <stddef.h>

namespace MultiStation{

	class MSSystemStack {
    public:
        MSSystemStack(void)noexcept;
        ~MSSystemStack(void)noexcept;

        void PushLayer(IMSSystem* layer)noexcept;
        void PushOverlay(IMSSystem* overlay)noexcept;
        void PopLayer(IMSSystem* layer)noexcept;
        void PopOverlay(IMSSystem* overlay)noexcept;

        std::vector<IMSSystem*>::iterator begin(void)noexcept;
        std::vector<IMSSystem*>::iterator end(void)noexcept;

    private:
        std::vector<IMSSystem*>     m_Systems;
        uint32_t                m_LastIndex;
	};

}
