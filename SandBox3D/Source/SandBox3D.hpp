#pragma once
#include "mspch.h"

namespace MultiStation {

	class SandBox3D : public Application {
	public:
		SandBox3D(uint32_t threads) noexcept;
		~SandBox3D(void) noexcept;

	public:

		virtual void SetUp(void) noexcept;


	};

}
