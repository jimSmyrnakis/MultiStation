#include "Camera.hpp"

namespace MultiStation::Component {

	Camera3D::Camera3D(void) : frame_buffer(Texture2DResolution()){
		fov = 65.0f;
		aspect_ratio = 1.0f;
		nearZ = 0.001f;
		farZ = 10000.0f;
		projection = glm::mat4(1);
		view = glm::mat4(1);
	}

	

}
