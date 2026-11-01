#pragma once
#include <mutex>
/**
 * @author Dimitris Smyrnakis
 * @file Handle.hpp
 * @brief This is where someone can find the definition and functionality of the io handle 
 * used for file and folders operations .
 * @version 0.1
 */
namespace MultiStation {

	namespace IO {

		/**
		 * @class Handle
		 * @brief Contains all necessery functionality and locks for using in multiple threads 
		 * , usefull for accessing it with synchronous and asynchronous io operations . 
		 * @warning Anyone want to access the handle for file operations should try lock it first 
		 * to achieve thread safe operation for io . Otherwise behavior may be undefined and will depend 
		 * on the file system thread safety . 
		 */
		struct Handle {

		public:

			Handle(void);

			/**
			 * @brief Locks the handle by a single thread that corrently want to uses it 
			 * for io operation . 
			 * 
			 */
			void Lock(void);

			/**
			 * @brief Locks the handle by a single thread that corrently want to uses it 
			 * for io operation but in non blocking way
			 * 
			 * @return True if lock succeded , false otherwise . This can be used for keep
			 * iterating through io requests when synchronous and asynchronous operations 
			 * try work on the same file handle .  
			 */
			bool TryLock(void);

			/**
			 * @brief Unlocks the locked handle for let other thread to lock it and use it
			 * for io . 
			 */
			void Unlock(void);


			void SetNative(void* native);

			void SetID(int id);

			int GetID(void) const;

			void* GetNative(void) const;

		private:
			void* m_native = nullptr;
			int m_id;
			mutable std::mutex m_lock;
		};

	}

}
