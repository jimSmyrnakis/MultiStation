#include "Transform.hpp"
#include <glm/gtx/transform.hpp>

namespace MultiStation {
	

	void Transform3D::Update() {
		glm::mat4 identity(1);
		//glm::rotate(model, glm::radians(angleDeg * blend), glm::vec3(1.0f, 1.0f, 1.0f))
		glm::mat4 rX = glm::rotate(identity, rotation.x, glm::vec3(1,0,0));
		glm::mat4 rY = glm::rotate(identity, rotation.y, glm::vec3(0, 1, 0));
		glm::mat4 rZ = glm::rotate(identity, rotation.z, glm::vec3(0, 0, 1));
		glm::mat4 translate = glm::translate(position);
		glm::mat4 scaleMat = glm::scale(scale);
		glm::mat4 rot = rX * rY * rZ;
		transform = scaleMat * translate * rot;
	}

	glm::vec3 Transform3D::UpDirection(void) const { return glm::vec3(0, 1, 0); }
	glm::vec3 Transform3D::RightDirection(void) const { return glm::vec3(1, 0, 0); }
	glm::vec3 Transform3D::ForwardDirection(void) const { return glm::vec3(0,0 , 1); }


}
