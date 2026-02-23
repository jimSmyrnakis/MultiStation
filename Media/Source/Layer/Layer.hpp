#pragma once
#include "../Events/Event.hpp"

namespace MultiStation {

    class Layer {
    public:
        Layer(const std::string& name = "Layer") noexcept;
        virtual ~Layer(void) noexcept ;

        virtual void OnAttach(void) noexcept ; // when the layer is push in the stack trigger this method
        virtual void OnDetach(void) noexcept ; // when the layer is removed from the stack trigger this method
        virtual void OnUpdate(float deltaTime) noexcept ; // every time that infinite loops where the layer is , trigger this method
        virtual void OnEvent(Event& event)noexcept; // whenever a event is parse to a layer trigger this method
        virtual void OnImGuiRender(void)noexcept; // for the UI Rendering , this is where you create button panels , checkboxes for a given layer
        const std::string& GetName(void) const noexcept;

        // For exambe lets say u have a scene u want to render , then the On


    protected:
        std::string m_Name;
    };

}
