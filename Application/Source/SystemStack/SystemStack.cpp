#include "SystemStack.hpp"

namespace MultiStation {


    SystemStack::SystemStack(void) noexcept {
        m_LastIndex = 0;
    }

    SystemStack::~SystemStack(void) noexcept {
        
    }

    void SystemStack::PushLayer(ISystem* layer) noexcept {
        m_Systems.emplace(m_Systems.begin() + m_LastIndex, layer);
        m_LastIndex++;
       // layer->OnAttach();
    }

    void SystemStack::PushOverlay(ISystem* overlay) noexcept {
        m_Systems.emplace_back(overlay);
        //overlay->OnAttach();
    }

    void SystemStack::PopLayer(ISystem* layer) noexcept {
        std::vector<ISystem*>::iterator it = std::find(m_Systems.begin(), m_Systems.end(), layer);
        if (it == m_Systems.end())
            return;

        //layer->OnDetach();
        m_Systems.erase(it);
        m_LastIndex--;

    }

    void SystemStack::PopOverlay(ISystem* overlay) noexcept {
        std::vector<ISystem*>::iterator it = std::find(m_Systems.begin(), m_Systems.end(), overlay);
        if (it == m_Systems.end())
            return;

       // (*it)->OnDetach();
        m_Systems.erase(it);
    }

    std::vector<ISystem*>::iterator SystemStack::begin(void) noexcept { return m_Systems.begin(); }

    std::vector<ISystem*>::iterator SystemStack::end(void) noexcept { return m_Systems.end(); }

}