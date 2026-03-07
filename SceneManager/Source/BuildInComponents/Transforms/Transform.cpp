#include "Transform.hpp"
namespace MultiStation {
	
	Transform::Transform(void) noexcept : IComponent("Tranform", 8),
	m_transform(1) , position(1) , rotation(1) , scale(1){

	}

	void Transform::UpdateTransform(void) {
		glm::mat4 translationMat = glm::translate(position);
		glm::mat4 rotationMat = glm::rotate(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 scaleMat = glm::scale(scale);
		m_transform = translationMat * rotationMat * scaleMat;
	}

}
