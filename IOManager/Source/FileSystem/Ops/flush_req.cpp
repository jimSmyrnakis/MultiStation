#include "../IOReq.hpp"
#include "../Handle/HandleManager.hpp"
#include <errno.h>
namespace MultiStation {

	namespace IO {

		ErrorCode FlushReq(Request& req, Response& res) noexcept {
			res.operation = Operations::FLUSH;

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

				//call fflush
				size_t ress = fflush( native);
				if ((ress == EOF)) {
					// error or eof
					res.error = ErrorCode::OPERATION_FAILED;
					hnd->Unlock();
					return res.error;
				}

				res.count = ress;

			}
			hnd->Unlock();




			// prepare result
			res.handle = id;
			res.error = ErrorCode::SUCCESS;
			return res.error;
		}

	}

}