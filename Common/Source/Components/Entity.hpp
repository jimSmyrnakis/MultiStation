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

        class EntitySerializable : public ISerialize
        {
        public:
            ~EntitySerializable(void) = default;

            virtual bool Serialize(void* ref, IArchiveWriter* archive) override;

            virtual bool Deserialize(void* ref, IArchiveReader* archive) override;
        };
    }

}
