#pragma once
#include <stdint.h>
#include <stddef.h>

namespace MultiStation{

	enum ErrorCode : uint32_t {

		SUCCESS = 0,
		UNKNOWN_ERROR = 1,
		OPERATION_FAILED = 2,
		FAILED_OPEN ,
		ACCESS_DENIED ,
		FILE_NOT_EXISTS ,
		UNKNOWN_OPERATION,
		FILE_EXISTS ,
		MEMORY_ALLOC_FAILED ,
		BAD_PARAMS ,

		BAD_ID = 0xFFFFFFFF

	};

}
