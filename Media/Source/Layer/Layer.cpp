#include "Layer.hpp"
namespace MultiStation {
    Layer::Layer(const std::string& name) noexcept : m_Name(name) {}
    Layer::~Layer(void) noexcept {
        OnDetach();
    }

    void Layer::OnAttach(void) noexcept {}
    void Layer::OnDetach(void) noexcept {}
    void Layer::OnUpdate(float deltaTime) noexcept {}
    void Layer::OnEvent(Event& event) noexcept {}
    void Layer::OnImGuiRender(void) noexcept {}

    const std::string& Layer::GetName(void) const noexcept { return m_Name; }
}
