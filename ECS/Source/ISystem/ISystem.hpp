#pragma once
#include <stdint.h>
#include <stddef.h>
#include "../Registry/Registry.hpp"
namespace MultiStation{

	class ISystem {
	public:
		
		struct Dependencies {
			std::vector<uint32_t> waitSystems;
			// systems type id 
			std::unordered_map<uint32_t, bool > componentsUsage;
			// component type id to if is writing or reading
		};
	protected:
		ISystem(void) noexcept;
		virtual ~ISystem(void) noexcept = default;

		ISystem(const ISystem& cpy) noexcept = delete;
		ISystem& operator=(const ISystem& cpy) noexcept = delete;
	public:
		ISystem(ISystem&& move) noexcept = default;
		virtual ISystem& operator=(ISystem&& move) noexcept = default ;

		// Called only one time when the system is created
		virtual void OnAwake(void) = 0;
		
		// Called when the System is Enabled
		virtual void OnEnable(void) = 0;

		// Called when the System is Started , after the OnEnable
		virtual void OnStart(void ) = 0;

		// Called every frame
		virtual void OnUpdate( float dt ) = 0;

		// Called when the System is Disabled
		virtual void OnDisable(void) = 0;

		// Called when the System is Destroyed
		virtual void OnDestroy(void) = 0;
		
		
	public:

		template<typename SystemType>
		static uint32_t GetTypeID(void);

		template<typename SystemType>
		void WaitFor(void);

		template<typename T>
		void UseComponent(bool writeAccess);

		Dependencies GetDependencies(void) const;

	protected:
		uint32_t m_flags;
		
	private:
		Dependencies m_dependencies;
		static uint32_t s_typeID;
	};


	template<typename SystemType>
	static uint32_t ISystem::GetTypeID(void) {
		// force T to be derived from ISystem , but not ISystem itself
		static_assert(
			std::is_base_of<ISystem, SystemType>::value && 
			!std::is_same<ISystem, SystemType>::value,
			"T must be derived from ISystem, not ISystem itself!");

		static uint32_t id = ++s_typeID;

		return id;
	}


	template<typename SystemType>
	void ISystem::WaitFor(void) {
		static_assert(
			std::is_base_of<ISystem, SystemType>::value &&
			!std::is_same<ISystem, SystemType>::value,
			"T must be derived from ISystem, not ISystem itself!");

		uint32_t SysTypeID = SystemType::GetTypeID();
		// search the type id 
		auto it = std::find(
			m_dependencies.waitSystems.begin(),
			m_dependencies.waitSystems.end(),
			SysTypeID);
		if (it != m_dependencies.waitSystems.end()) {
			// TODO : Warning
			return;
		}
		m_dependencies.waitSystems.push_back(SysTypeID);
	}

	template<typename T>
	void ISystem::UseComponent(bool writeAccess) {
		static_assert(
			std::is_base_of<IComponent<T>, T>::value &&
			!std::is_same<IComponent<T>, T>::value,
			"T must be derived from IComponent<T>, not IComponent<T> itself!");

		uint32_t id = IComponent<T>::GetTypeID();

		// Search
		auto& map = m_dependencies.componentsUsage;
		if (map.find(id) != map.end()) {
			// TODO : Warning
			return;
		}

		map[id] = writeAccess;
	}


	ISystem::Dependencies ISystem::GetDependencies(void) const {
		return m_dependencies;
	}
}
