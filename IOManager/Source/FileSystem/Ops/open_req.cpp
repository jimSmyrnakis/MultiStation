#include "../IOReq.hpp"
#include "../Handle/HandleManager.hpp"
namespace MultiStation {

	namespace IO {
		std::string GetFMode(Access access) {
			Access actuall;
			if (HasAccess(access, Access::APPEND) && HasAccess(access ,  Access::WRITE)) {
				actuall = Access::APPEND;
				if (HasAccess(access, Access::READ))
					actuall = actuall | Access::READ;
			}
			else {
				actuall = access;
			}

			
			std::string mode = "";
			uint16_t act = (uint16_t)actuall;
			if (actuall == Access::READ) {
				mode = "rb";
			}
			else if (actuall == Access::WRITE) {
				mode = "wb";
			}
			else if (actuall == Access::APPEND) {
				mode = "ab";
			}
			else if (IsEqual(actuall, Access::READ | Access::WRITE)) {
				mode = "rb+";
			}
			else if (IsEqual(actuall, Access::READ | Access::APPEND)) {
				mode = "ab+";
			}
			else {
				MS_ASSERT(false, "Oh ho");
			}


			
			return mode;
		}

		ErrorCode OpenReq(Request& req, Response& res) noexcept {
			res.operation = Operations::OPEN;

			// prepare handle 
			int id = HandleManager::CreateHandle(nullptr);
			std::shared_ptr<Handle> hnd = HandleManager::GetHandle(id);
			if (!hnd) {
				res.error = ErrorCode::OPERATION_FAILED;
				return res.error;
			}

			// do fopen for file existance checking
			FILE* ncheck = fopen(req.file_path.c_str(), "r");
			if (ncheck == NULL) {

				if (errno == ENOENT) {
					res.error = ErrorCode::FILE_NOT_EXISTS;
				}
				else {
					res.error = ErrorCode::OPERATION_FAILED;
				}
				HandleManager::RemoveHandle(id);
				return res.error;
			}
			fclose(ncheck);


			// do actuall fopen
			std::string mode = GetFMode(req.access);
			FILE* native = fopen(req.file_path.c_str(), mode.c_str());

			if (native == NULL) {

				if (errno == ENOENT) {
					res.error = ErrorCode::FILE_NOT_EXISTS;
				}
				else {
					res.error = ErrorCode::OPERATION_FAILED;
				}
				HandleManager::RemoveHandle(id);
				return res.error;
			}
			rewind(native);

			// assign handle native and id 
			hnd->Lock();
			hnd->SetNative(native);
			hnd->SetID(id);
			hnd->Unlock();


			// prepare result
			res.handle = id;
			res.error = ErrorCode::SUCCESS;
			return res.error;
		}

	}

}