#pragma once
#include <stdint.h>
#include <stddef.h>
#include "../ComponentRegistry/ComponentRegistry.hpp"
#include "../ManagerControlBlock/ManagerControlBlock.hpp"
namespace MultiStation{

	class ISystem {
	public:
		// Flags
		static uint32_t WRITABLE;// = 0x00000001; if this system may change the registry
		static uint32_t SYNCHRONIZED;// = 0x00000002; if the system may not need to run 
		// at the same game loop a the others , input system may belong there 
	protected:
		ISystem(uint32_t flags) noexcept;
		~ISystem(void) noexcept = default;

		ISystem(const ISystem& cpy) noexcept = delete;
		ISystem& operator=(const ISystem& cpy) noexcept = delete;
	public:
		ISystem(ISystem&& move) noexcept = default;
		virtual ISystem& operator=(ISystem&& move) noexcept = default ;


		// Called every time this system is loaded in the scene 
		// , the system send's if the registry is changed 
		virtual void OnCreate(ComponentRegistry* registry ) = 0;

		// Called every Hole System Loop , or Game Loop , or Scene loop
		// When the System is Called , the dirty says to him if the Registry has
		// changed . When The System finished then it must update the dirty flag
		virtual void OnUpdate(ComponentRegistry* registry, float dt , ManagerControlBlock* controlBlock) = 0;

		// Called when the System is Destroyed
		virtual void OnDestroy(ComponentRegistry* registry, ManagerControlBlock* controlBlock) = 0;
		

		virtual uint32_t GetFlags(void) const;

		virtual void SetFlags(uint32_t flags);


	protected:
		uint32_t m_flags;
		
	};

}
