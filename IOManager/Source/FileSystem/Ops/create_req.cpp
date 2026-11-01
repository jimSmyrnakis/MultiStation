#include "../IOReq.hpp"
#include "../Handle/HandleManager.hpp"
#include <errno.h>
namespace MultiStation {

	namespace IO {




		/*
		* "a"	append	Append to a file	write to end	create new
		* "w+"	write extended	Create a file for read/write	destroy contents	create new
		*/




		ErrorCode CreateReq(Request& req, Response& res) noexcept {
			res.operation = Operations::CREATE;
			const char* mode = (req.operation_flags & EMPTY_IF_EXISTS) ? "w+" :  "a";
			FILE* native = fopen(req.file_path.c_str(),mode);

			if (native == NULL) {
				
				if (errno == ENOENT) {
					res.error = ErrorCode::FILE_NOT_EXISTS;
				}
				else {
					res.error = ErrorCode::OPERATION_FAILED;
				}
				
				
				return res.error;
			}

			fclose(native);
			res.pos = 0;
			

			res.error = ErrorCode::SUCCESS;
			return res.error;
		}

	}

}