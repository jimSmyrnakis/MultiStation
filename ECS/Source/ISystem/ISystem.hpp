#pragma once
#include <stdint.h>
#include <stddef.h>
#include "../Registry/Registry.hpp"
#include "../JobSystem/JobSystem.hpp"
#include "../ECSManager/ECSManager.hpp"
namespace MultiStation{

	struct SystemContext {
		std::shared_ptr<JobSystem> jobSystem;
		float deltaTime;
		std::shared_ptr<ECSManager> ecs;
	};

	class ISystem {
	
	protected:
		ISystem(void) noexcept = default;
		virtual ~ISystem(void) noexcept = default;

		ISystem(const ISystem& cpy) noexcept = delete;
		ISystem& operator=(const ISystem& cpy) noexcept = delete;
	public:
		ISystem(ISystem&& move) noexcept = default;
		virtual ISystem& operator=(ISystem&& move) noexcept = default ;

		

		// Called every new iteration
		virtual void OnTick(SystemContext* ctx) = 0;

		
		
		
	public:

		template<typename SystemType>
		static uint32_t GetTypeID(void);
		
	private:
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




}
