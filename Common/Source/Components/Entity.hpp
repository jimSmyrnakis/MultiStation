#pragma once
#include <ECS.hpp>
#include <Media.hpp>
#include <ImGui.hpp>
#include <Platform.hpp>
#include <string>
namespace MultiStation{
    namespace Component {
        struct Entity {
            EntID id;
            char name[180];
            UUID handle;
            Entity(void);


        };

        class EntityInspectable : public IInspectable
        {
        public:
            void DrawElement(void* data) override;
            
        };
    }

}
