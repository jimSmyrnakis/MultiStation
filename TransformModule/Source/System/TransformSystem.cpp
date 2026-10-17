#include "TransformSystem.hpp"
#include <Common.hpp>
#include <GLM.hpp>
namespace MultiStation {

	TransformSystem::TransformSystem(EngineContext& ctx) : m_context(ctx) {
	
	}

	void TransformSystem::OnAttach(void) {

	}

	void TransformSystem::OnDettach(void) {

	}







	void CaculateTransform(Component::Transform3D& transform);

	void TransformSystem::OnUpdate(float dt) {
		

		// for each entity caculate the transform
		auto transforms = 
			m_context.GetCurrentScene()->GetComponents<Component::Transform3D>();

		for (auto& transform : transforms) {
			CaculateTransform(transform);
		}
	}

	void CaculateTransform(Component::Transform3D& transform) {
		glm::mat4 identity{ 1.0f };

		glm::mat4 translation =
			glm::translate(identity, transform.position);

		glm::mat4 rotationX =
			glm::rotate(identity, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));

		glm::mat4 rotationY =
			glm::rotate(identity, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));

		glm::mat4 rotationZ =
			glm::rotate(identity, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));

		glm::mat4 scaling =
			glm::scale(identity, transform.scale);

		transform.local_matrix =
			translation * rotationZ * rotationY * rotationX * scaling;

		transform.word_matrix = transform.local_matrix;
	}














	void TransformSystem::OnEvent(Event& e) {

	}

}
