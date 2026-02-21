#pragma once
#include <stdint.h>
#include <stddef.h>
#include <unordered_map>
#include <cassert>
#include "../VersionControl.hpp"
#include <functional>
#include <Platform.hpp>
#include <string>
namespace MultiStation {

	
	/**
	 *
	 */
	template<typename Derived>
	class IComponent {
		// compile-time check ότι Derived είναι ίδιος με τον actual τύπο του this
//		static_assert(!std::is_base_of<IComponent<Derived>, Derived>::value,
//			"Derived must inherit from IComponent<Derived>");
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

		std::string& GetName(void) noexcept;

		const std::string& GetName(void) const noexcept;


	private:
		VersionControl m_versionControl;
		static std::atomic<uint32_t> s_typeID;
		uint32_t m_instanceTypeID;
		std::string m_name;
	};


	template<typename Derived>
	std::atomic<uint32_t> IComponent<Derived>::s_typeID = 0;



















	template<typename Derived>
	uint32_t IComponent<Derived>::GetVersion(void) const {
		return m_versionControl.GetVersion();
	}

	template<typename Derived>
	void IComponent<Derived>::SetDirty(void) const {


		// increment version control counter
		m_versionControl.IncrementVersion();
	}

	

	template<typename Derived>
	uint32_t IComponent<Derived>::GetInstanceTypeID(void) const {

		return m_instanceTypeID;
	}


	template<typename Derived>
	IComponent<Derived>::IComponent(void) noexcept {
		MS_ASSERT(typeid(*this) == typeid(Derived) , "Can't use IComponent base instance");
		m_instanceTypeID= 
			s_typeID.fetch_add(1, std::memory_order_relaxed);
		m_name = typeid(Derived).name();
	}


	template<typename Derived>
	std::string& IComponent<Derived>::GetName(void) noexcept {
		return m_name;
	}

	template<typename Derived>
	const std::string& IComponent<Derived>::GetName(void) const noexcept {
		return m_name;
	}

}

