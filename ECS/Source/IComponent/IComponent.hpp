#pragma once
#include <stdint.h>
#include <stddef.h>
#include <unordered_map>
#include <cassert>
#include "../VersionControl.hpp"
#include <functional>
namespace MultiStation {

	
	/**
	 *
	 */
	template<typename Derived>
	class IComponent {
		// compile-time check ότι Derived είναι ίδιος με τον actual τύπο του this
		static_assert(std::is_base_of<IComponent<Derived>, Derived>::value,
			"Derived must inherit from IComponent<Derived>");
	public:

		IComponent(void) noexcept;
		virtual ~IComponent(void) noexcept = default;

		IComponent(const IComponent& cpy) noexcept = delete;
		IComponent& operator=(const IComponent& cpy) noexcept = delete;

		IComponent(IComponent&& move) noexcept  = default;
		virtual IComponent& operator=(IComponent&& move) noexcept = default;

		uint32_t GetInstanceTypeID(void) const;

		uint32_t GetVersion(void) const;

		void SetDirty(void) const;

		void SetCallBack(std::function<void(uint32_t)> callback);

	public:
		static uint32_t GetTypeID(void);

	private:
		VersionControl m_versionControl;
		std::function<void(uint32_t instanceTypeID)> m_dirtyCallBack;
		static uint32_t s_typeID;
		static std::unordered_map<uint32_t, uint32_t> s_typeToIDMap;
		uint32_t m_instanceTypeID;
	};
























	template<typename Derived>
	uint32_t IComponent<Derived>::s_typeID = 0;

	template<typename Derived>
	std::unordered_map<uint32_t, uint32_t> IComponent<Derived>::s_typeToIDMap;

	template<typename Derived>
	void IComponent<Derived>::SetCallBack(std::function<void(uint32_t)> callback) {
		m_dirtyCallBack = callback;
	}

	template<typename Derived>
	uint32_t IComponent<Derived>::GetVersion(void) const {
		return m_versionControl.GetVersion();
	}

	template<typename Derived>
	void IComponent<Derived>::SetDirty(void) const {

		if (m_dirtyCallBack) {
			m_dirtyCallBack(this->GetInstanceTypeID());
		}

		// increment version control counter
		m_versionControl.IncrementVersion();
	}


	template<typename Derived>
	uint32_t IComponent<Derived>::GetTypeID(void) {
		static uint32_t id = ++s_typeID;
		return id;
	}

	

	

	template<typename Derived>
	uint32_t IComponent<Derived>::GetInstanceTypeID(void) const {

		return m_instanceTypeID;
	}


	template<typename Derived>
	IComponent<Derived>::IComponent(void) noexcept {
		assert(typeid(*this) == typeid(Derived));
		m_instanceTypeID = ++s_typeToIDMap[IComponent<Derived>::GetTypeID()];
	}

}

