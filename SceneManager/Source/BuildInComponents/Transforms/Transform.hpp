#pragma once
#include <GLM.hpp>
#include "../../Interfaces/IComponent.hpp"
namespace MultiStation {
	class Transform : public IComponent {
	public:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		
		
		Transform(void) noexcept ;
	

		void UpdateTransform(void);

		
	private:
		glm::mat4 m_transform;
	};
}
