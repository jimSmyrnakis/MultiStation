#pragma once

#include "../Definitions.hpp"

namespace MultiStation {
	class Time {
	public:
		static float GetTimeInSeconds(void);
		static uint64_t GetTimeInMilliseconds(void);
	};
}
