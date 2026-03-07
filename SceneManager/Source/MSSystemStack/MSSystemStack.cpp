#include "MSSystemStack.hpp"

namespace MultiStation {


    MSSystemStack::MSSystemStack(void) noexcept {
        m_LastIndex = 0;
    }

    MSSystemStack::~MSSystemStack(void) noexcept {
        
    }

    void MSSystemStack::PushLayer(IMSSystem* layer) noexcept {
        m_Systems.emplace(m_Systems.begin() + m_LastIndex, layer);
        m_LastIndex++;
    }

    void MSSystemStack::PushOverlay(IMSSystem* overlay) noexcept {
        m_Systems.emplace_back(overlay);
    }

    void MSSystemStack::PopLayer(IMSSystem* layer) noexcept {
        std::vector<IMSSystem*>::iterator it = std::find(m_Systems.begin(), m_Systems.end(), layer);
        if (it == m_Systems.end())
            return;

        m_Systems.erase(it);
        m_LastIndex--;

    }

    void MSSystemStack::PopOverlay(IMSSystem* overlay) noexcept {
        std::vector<IMSSystem*>::iterator it = std::find(m_Systems.begin(), m_Systems.end(), overlay);
        if (it == m_Systems.end())
            return;

        m_Systems.erase(it);
    }

    std::vector<IMSSystem*>::iterator MSSystemStack::begin(void) noexcept { return m_Systems.begin(); }

    std::vector<IMSSystem*>::iterator MSSystemStack::end(void) noexcept { return m_Systems.end(); }

}