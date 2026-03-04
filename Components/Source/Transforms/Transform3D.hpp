#pragma once
#include <GLM.hpp>
#include <ECS.hpp>
namespace MultiStation{
	struct Transform3D {

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		
		
		Transform3D(void) noexcept = default;
	
		Transform3D(const Transform3D& cpy) = delete;
		Transform3D& operator=(const Transform3D& cpy) = delete;
		Transform3D(Transform3D&& move) noexcept;
		Transform3D& operator=(Transform3D&& move) noexcept;

		void UpdateTransform(void);

		
	private:
		glm::mat4 m_transform;
	};
}
