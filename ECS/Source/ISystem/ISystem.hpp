#pragma once
#include <stdint.h>
#include <stddef.h>
#include "../ComponentRegistry/ComponentRegistry.hpp"
namespace MultiStation{

	class ISystem {
	public:
		static uint32_t WRITABLE;// = 0x00000001;
	protected:
		ISystem(uint32_t flags) noexcept;
		~ISystem(void) noexcept = default;

		ISystem(const ISystem& cpy) noexcept = delete;
		ISystem& operator=(const ISystem& cpy) noexcept = delete;

		ISystem(ISystem&& move) noexcept = default;
		virtual ISystem& operator=(ISystem&& move) noexcept = default ;


		// Called every time this system is loaded in the scene 
		// , the system send's if the registry is changed 
		virtual void OnCreate(ComponentRegistry* registry ) = 0;

		// Called every Hole System Loop , or Game Loop , or Scene loop
		// When the System is Called , the dirty says to him if the Registry has
		// changed . When The System finished then it must update the dirty flag
		virtual void OnUpdate(ComponentRegistry* registry, float dt , bool* dirty) = 0;

		// Called when the System is Destroyed
		virtual void OnDestroy(ComponentRegistry* registry, bool* dirty) = 0;
		

		uint32_t GetFlags(void) const;

		void SetFlags(uint32_t flags);


	protected:
		uint32_t m_flags;
		
	};

}
