#include "Transform3D.hpp"
namespace MultiStation {
	

	Transform3D::Transform3D(Transform3D&& move) noexcept {
		position	= std::move(move.position)	;
		rotation	= std::move(move.rotation)	;
		scale		= std::move(move.scale)		;
		m_transform = std::move(move.m_transform);

	}
	Transform3D& Transform3D::operator=(Transform3D&& move) noexcept {
		position = std::move(move.position);
		rotation = std::move(move.rotation);
		scale = std::move(move.scale);
		m_transform = std::move(move.m_transform);
		return *this;
	}

	void Transform3D::UpdateTransform(void) {
		glm::mat4 translationMat = glm::translate(position);
		glm::mat4 rotationMat = glm::rotate(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 scaleMat = glm::scale(scale);
		m_transform = translationMat * rotationMat * scaleMat;
	}

}
