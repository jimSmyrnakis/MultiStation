#pragma once
#include <Utilities.hpp>
#include "../Globals.hpp"
#include <memory>
/**
 * @author Dimitris Smyrnakis
 * @file IOManager.hpp 
 * @brief This is where the class for io synchronous and asynchronous operations exist's
 * @version 0.1
 */
namespace MultiStation{

	namespace IO {

		/**
		 * @class IOManager
		 * @brief Handles synchronous and asynchronous io operations , for files and folders . 
		 * Fethermore io manager is executed / updated to an io thread different from the main thread .
		 * @note Request's and Result's memory must not be stack addresses and the lifetime is defined 
		 * by the shared pointer , as long one of two or more sides have the same address wrap around the 
		 * shared pointer the object remains on memory valid . 
		 * @warning This class should not used by the end system developer , instead use the IO:: sync and 
		 * async operations that wrap around a instance of this class .  
		 */
		class IOManager {

		private:

			IOManager(void) = default;

			~IOManager(void) = default;
		public:

			/**
			 * @brief Push a new io requests to its queue of requests
			 * @param reqio The new request shared pointer 
			 * @return True if reqio looks valid , false otherwise 
			 */
			bool PushRequest(std::shared_ptr<Request>& reqio );

			/**
			 * @brief This method used for synchronous io operations and it handles all the 
			 * thread request handling beetween the two different threads .
			 * 
			 * @param reqio The request shared pointer for the sync io 
			 * @param resio The result shared pointer from the sync io 
			 * @return True if request is valid operation otherwise false .
			 */
			bool ExecuteSync(std::shared_ptr<Request>& reqio, std::shared_ptr < Result>& resio);

			/**
			 * @brief This method returns all the first in the queue finished result for us 
			 * to handle . This is used on the main thread loop for executing the callbacks 
			 * in a thread safe way on each game loop .
			 * 
			 * @param resio The result shared pointer 
			 * @return True if there is a result pending on the queue or false otherwise . 
			 */
			bool PopResult(std::shared_ptr<Result>& resio );

			/**
			 * @brief This method is used by the io thread for updating the io manager to execute the new 
			 * requests . This loop is in async with the game / main loop and that what should be as it handles
			 * all the io requests in a synchronous manner .
			 * 
			 */
			void UpdateIO(void);

			/**
			 * @brief This method is used again by the io thread for the execution of all pending request's
			 * Then for each handle it closes the open files to ensure that the results are saved . 
			 */
			void Fini(void);

			/**
			 * @return Singleton class IO Manager  
			 */
			static IOManager& Get(void);

		private:

			void ExecuteRequest(Request& request, Result& result);

		private:
			
			Queue<std::shared_ptr<Request>> m_requestsQueue;
			static IOManager s_manager;
			Queue<std::shared_ptr<Result>>  m_resultsQueue;
			std::vector<std::shared_ptr<Handle>> m_handels;
		};

	}

}
