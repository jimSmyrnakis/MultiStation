#pragma once
#include <vector>
#include <stdint.h>
#include <stddef.h>
namespace MultiStation{

	class Task {
	

	public:
		virtual ~Task(void) noexcept = default;
		virtual void Run(uint16_t threadID) = 0;

		
	};

	

}
