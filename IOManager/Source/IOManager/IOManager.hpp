#pragma once
#include <Utilities.hpp>
#include <vector>
#include "../Globals.hpp"

/**
 * @author Dimitris Smyrnakis
 */
namespace MultiStation{

	namespace IO {
		
		

		

		

		class IOManager {

		public:

			bool PushRequest(Request& req) noexcept;

			bool HasPendingRequests(void) noexcept;

			void Update(void) noexcept;

			void PollIO(void) noexcept;


		private:
			
			Queue<Request> m_requests;
			Queue<Response> m_responses;

		};

	}

}
