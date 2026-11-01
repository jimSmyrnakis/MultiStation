#include "../IOReq.hpp"
#include "../Handle/HandleManager.hpp"
namespace MultiStation {

	namespace IO {

		ErrorCode CloseReq(Request& req, Response& res) noexcept {
			res.operation = Operations::CLOSE;
			std::shared_ptr<Handle> handle = HandleManager::GetHandle(req.handle);
			{

			}
			if (!handle) {

				res.error =  ErrorCode::OPERATION_FAILED;
				return res.error;
			}

			handle->Lock();

			if (!handle->GetNative()) {
				handle->Unlock();
				res.error = ErrorCode::OPERATION_FAILED;
				HandleManager::RemoveHandle(req.handle);
				return res.error;
			}

			FILE* native = (FILE*) handle->GetNative();
			int response = fclose(native);
			handle->SetNative(nullptr);
			HandleManager::RemoveHandle(req.handle);
			handle->Unlock();
			
			 
			res.error = ErrorCode::SUCCESS;
			return res.error;
		}

	}

}