#include "Job.hpp"
#include <string.h>
namespace MultiStation {

	uint32_t Job::s_nextID = 0;
	Job::Job(void) noexcept {
		m_id = 
		s_nextID++;
		affine = false;
		affinityWorkerID = 0;
		strncpy_s(name, "Unnamed Job" , 64);
		blockID = 0;
		fun = NULL;
		data = NULL;
		blockSize = 0;
		counter = nullptr;
		
		
	}
	uint32_t Job::GetID(void) const noexcept {
		return m_id;
	}


}
