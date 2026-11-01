#include "IOManager.hpp"

namespace MultiStation {

	namespace IO {

		

		bool IOManager::PushRequest(Request& req) noexcept {
			m_requests.Push(req);
			return true;
		}

		bool IOManager::HasPendingRequests(void) noexcept{
			return m_responses.Size() != 0;
		}

		void IOManager::Update(void) noexcept{
			while (m_requests.Size()) {
				
			}
		}

		void IOManager::PollIO(void) noexcept{

		}

	}

}
