#include "../IOReq.hpp"
#include "../Handle/HandleManager.hpp"
#include <errno.h>
namespace MultiStation {

	namespace IO {

		ErrorCode TellReq(Request& req, Response& res) noexcept {
			res.operation = Operations::TELL;

			// prepare handle 
			int id = req.handle;
			std::shared_ptr<Handle> hnd = HandleManager::GetHandle(id);
			if (!hnd) {
				res.error = ErrorCode::BAD_ID;
				return res.error;
			}

			// do file operation
			hnd->Lock();
			{
				// read native handle
				FILE* native = (FILE*)hnd->GetNative();
				if (!native) {
					res.error = ErrorCode::BAD_ID;
					hnd->Unlock();
					return res.error;
				}

				//call fseek
				long ress = ftell(native);
				if ((ress == -1L)) {
					// error
					res.error = ErrorCode::OPERATION_FAILED;
					hnd->Unlock();
					return res.error;
				}
				res.pos = ress;

			}
			hnd->Unlock();




			// prepare result
			res.handle = id;
			res.error = ErrorCode::SUCCESS;
			return res.error;
		}

	}

}