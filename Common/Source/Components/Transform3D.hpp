#pragma once
#include <GLM.hpp>
#include <Media.hpp>
#include <ImGui.hpp>
#include <Platform.hpp>
#include <ECS.hpp>
#include <string>
namespace MultiStation{

	namespace Component {

		struct Transform3D {
			glm::vec3 position ;
			glm::vec3 rotation;
			glm::vec3 scale;
			glm::mat4 local_matrix;
			glm::mat4 word_matrix;
			Transform3D(void);
		};


		class Transform3DInspectable : public IInspectable {
		public:
			virtual ~Transform3DInspectable(void) = default;

			void DrawElement(void* data) override;


		};

		class Transform3DSerializable : public ISerialize {
		public:
			~Transform3DSerializable(void) = default;

			virtual bool Serialize(void* ref, IArchiveWriter* archive) override;

			virtual bool Deserialize(void* ref, IArchiveReader* archive) override;
		};
	}

}
