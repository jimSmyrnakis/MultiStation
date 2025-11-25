#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
namespace MultiStation{
	struct Transform3D {

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::mat4 transform;
		
		void Update();
		
		glm::vec3 UpDirection(void) const;
		glm::vec3 RightDirection(void) const;
		glm::vec3 ForwardDirection(void) const;
	

	};
}
