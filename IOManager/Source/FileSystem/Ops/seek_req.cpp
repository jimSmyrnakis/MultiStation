#include "../IOReq.hpp"
#include "../Handle/HandleManager.hpp"
#include <errno.h>
namespace MultiStation {

	namespace IO {

		int SeekFMode(SeekMode mode) {
			switch (mode) {
			case SeekMode::END:
				return SEEK_END;
			case SeekMode::START:
				return SEEK_SET;
			case SeekMode::CURRENT:
				return SEEK_CUR;
			default:
				return SEEK_SET;
			}
			return SEEK_SET;
		}

		ErrorCode SeekReq(Request& req, Response& res) noexcept {
			res.operation = Operations::SEEK;

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
				size_t ress = fseek(native , req.pos , SeekFMode(req.seek_mode));
				if ((ress != 0)) {
					// error
					res.error = ErrorCode::OPERATION_FAILED;
					hnd->Unlock();
					return res.error;
				}
				res.pos = ftell(native);

			}
			hnd->Unlock();




			// prepare result
			res.handle = id;
			res.error = ErrorCode::SUCCESS;
			return res.error;
		}

	}

}