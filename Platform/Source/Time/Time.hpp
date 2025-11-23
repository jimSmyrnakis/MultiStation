#pragma once

#include "../Definitions.h"

namespace MultiStation {
	class Time {
	public:
		static float GetTimeInSeconds(void);
		static uint64_t GetTimeInMilliseconds(void);
	};
}
