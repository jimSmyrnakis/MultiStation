#include "../IOReq.hpp"
#include "../Handle/HandleManager.hpp"
#include <errno.h>
namespace MultiStation {

	namespace IO {


		ErrorCode ReadReq(Request& req, Response& res) noexcept {
			res.operation = Operations::READ;

			// check request
			if (req.count == 0 || req.buffer == nullptr) {
				res.error = ErrorCode::BAD_PARAMS;
				return res.error;
			}

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

				//call fread
				size_t ress = fread(req.buffer, 1 ,req.count , native);
				int reof = feof(native);
				if ( ((ress == 0) && !reof) || (ferror(native)) ) {
					// error or eof
					res.error = ErrorCode::OPERATION_FAILED;
					hnd->Unlock();
					return res.error;
				}

				res.eof = reof;
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
