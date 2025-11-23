#pragma once
#include <glm/glm.hpp>
namespace MultiStation{

	class Transform {

	public:
		Transform(void) noexcept;
		~Transform(void) = delete;

		void SetTransformMatrix(glm::mat4& matrix);
		glm::mat4& GetTransformMatrix(void) const;
		
		void Rotate(const glm::vec3& eulerAngles);
		void Translate(const glm::vec3& position);
		void Scale(const glm::vec3& scale);

		glm::vec3 UpDirection(void) const;
		glm::vec3 RightDirection(void) const;
		glm::vec3 FrontDirection(void) const;


	private:
		glm::vec3 m_Position;
		glm::vec3 m_EulerAngles;
		glm::vec3 m_Scale;
		glm::mat4 m_TranformMatrix;
	};

}
