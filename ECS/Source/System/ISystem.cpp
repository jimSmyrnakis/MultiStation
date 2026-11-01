#include "ISystem.hpp"

namespace MultiStation {

	void ISystem::OnAttach(void) noexcept {

	}

	void ISystem::OnEnable(void) noexcept {

	}

	void ISystem::OnUpdate(float delta_time) noexcept {

	}


	void ISystem::OnDisable(void) noexcept {

	}

	void ISystem::OnDettach(void) noexcept {

	}


	
	






#define MIN_FIXED_DELTA 1.0f / 60.0f


	ISystem::ISystem(void) noexcept {
		m_fixedDeltaTime = MIN_FIXED_DELTA;
		m_enable = true;
		m_name = "System";
	}






	void ISystem::SetEnable(bool enable){
		m_enable = enable;
	}

	bool ISystem::IsEnable(void) const{
		return m_enable;
	}

	bool ISystem::SetFixedDeltaTime(float deltaTime){
		if (deltaTime >= MIN_FIXED_DELTA) {
			m_fixedDeltaTime = deltaTime;
			return true;
		}

		return false;
	}

	float ISystem::GetFixedDeltaTime(void) const{
		return m_fixedDeltaTime;
	}

	void ISystem::SetName(const std::string& name){
		m_name = name;
	}

	const std::string& ISystem::GetName(void) const{
		return m_name;
	}

}
