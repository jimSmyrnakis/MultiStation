#pragma once
#include <mutex>
#include <unordered_map>
#include <memory>
#include "Handle.hpp"
namespace MultiStation {

	namespace IO {





		class HandleManager {


		public:

			static int CreateHandle(void* native);

			static void RemoveHandle(int id);

			static std::shared_ptr<Handle> GetHandle(int id);

			static void UnlockHandle(int id);

			static bool HasHandle(int id);

			static std::unordered_map<int, std::shared_ptr<Handle>> s_handles;
			static std::mutex s_lock;
		};

	};

};
