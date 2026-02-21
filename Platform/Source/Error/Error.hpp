#pragma once
#include <stdint.h>
#include <stddef.h>

namespace MultiStation{

	enum ErrorCode : uint32_t {

		SUCCESS = 0,
		UNKNOWN_ERROR = 1,

		BAD_ID = 0xFFFFFFFF

	};

}
