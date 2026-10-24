#include "TransformSystem.hpp"
#include <Common.hpp>
#include <GLM.hpp>
namespace MultiStation {

	TransformSystem::TransformSystem(EngineContext& ctx) : m_context(ctx) {
	
	}

	void TransformSystem::OnAttach(void) noexcept {
		MS_INFO("Transform System On Attach");
	}

	void TransformSystem::OnDettach(void) noexcept {
		MS_INFO("Transform System On Dettach");
	}







	void CaculateTransform(Component::Transform3D& transform);

	void RecursiveWorldMats(EntID entity, EngineContext& context) {

		// for each of its childs , call them
		auto childrens = context.GetCurrentScene()->GetChildren(entity);
		auto* ent_tr = 
			context.GetCurrentScene()->GetComponent<Component::Transform3D>(entity);
		for (EntID child : childrens) {
			auto* tr =
				context.GetCurrentScene()->GetComponent<Component::Transform3D>(child);
			tr->word_matrix = ent_tr->word_matrix * tr->local_matrix;
			RecursiveWorldMats(child, context);
			
		}
	}

	void TransformSystem::OnUpdate(float dt) noexcept {
		

		// for each entity caculate the transform
		auto transforms = 
			m_context.GetCurrentScene()->GetComponents<Component::Transform3D>();
		// caculate all local transforms
		for (auto& transform : transforms) {
			CaculateTransform(transform);
		}

		// now caculate all world transforms
		auto root_ents = m_context.GetCurrentScene()->GetChildren(rootEntity);
		for (EntID rootEnt : root_ents) {
			RecursiveWorldMats(rootEnt, m_context);
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















}
