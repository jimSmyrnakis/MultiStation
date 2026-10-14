#pragma once
#include <Utilities.hpp>
#include "../Globals.hpp"
namespace MultiStation{

	namespace IO {

		class IOManager {

		public:

			IOManager(void) = default;

			~IOManager(void) = default;

			bool PushRequest(Request* reqio );

			bool ExecuteSync(Request* reqio, Result* resio);

			bool PopResult(Result*& resio );

			void UpdateIO(void);

			void Fini(void);

		private:

			bool ExecuteRequest(Request& request, Result& result);

		private:
			
			Queue<Request*> m_requestsQueue;

			Queue<Result*>  m_resultsQueue;
			std::vector<Handle*> m_handels;
		};

	}

}
