#pragma once
#include <ECS.hpp>
#include <vector>
#include <stdint.h>
#include <stddef.h>

namespace MultiStation{

	class SystemStack {
    public:
        SystemStack(void)noexcept;
        ~SystemStack(void)noexcept;

        void PushLayer(ISystem* layer)noexcept;
        void PushOverlay(ISystem* overlay)noexcept;
        void PopLayer(ISystem* layer)noexcept;
        void PopOverlay(ISystem* overlay)noexcept;

        std::vector<ISystem*>::iterator begin(void)noexcept;
        std::vector<ISystem*>::iterator end(void)noexcept;

    private:
        std::vector<ISystem*>     m_Systems;
        uint32_t                m_LastIndex;
	};

}
