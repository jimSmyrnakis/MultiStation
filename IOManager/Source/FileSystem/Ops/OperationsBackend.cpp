#include "../IOReq.hpp"
namespace MultiStation::IO {



	ErrorCode ExecuteOperation(Request& req, Response& res) noexcept {
		switch (req.operation) {
			case Operations::OPEN: {
				return OpenReq(req, res);
			}
			case Operations::CREATE: {
				return CreateReq(req, res);
			}
			case Operations::READ: {
				return ReadReq(req, res);
			}
			case Operations::WRITE: {
				return WriteReq(req, res);
			} 
			case Operations::FLUSH: {
				return FlushReq(req, res);
			}
			case Operations::SEEK: {
				return SeekReq(req, res);
			}
			case Operations::TELL: {
				return TellReq(req, res);
			}
			case Operations::CLOSE: {
				return CloseReq(req, res);
			}
			case Operations::READ_DIR: {
				return ReadDirReq(req, res);
			}
			case Operations::MAKE_DIR: {
				return MakeDirReq(req, res);
			}
			case Operations::RMVE_DIR: {
				return RmveDirReq(req, res);
			}
			case Operations::INFO: {
				return InfoReq(req, res);
			}
			default: {
				return ErrorCode::UNKNOWN_OPERATION;
			}

		}
		return ErrorCode::UNKNOWN_OPERATION;
	}
}


