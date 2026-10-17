#pragma once
#include <GLM.hpp>
#include <Media.hpp>
namespace MultiStation{

	namespace Component {

		struct Camera3D {
			FBuffer frame_buffer;
			glm::mat4 view;
			glm::mat4 projection;
			float aspect_ratio;
			float fov;
			float nearZ;
			float farZ;
			Camera3D(void);
		};

		class Camera3DInspectable : public IInspectable
		{
		public:
			void DrawElement(void* data) override;

		};

	}

}
