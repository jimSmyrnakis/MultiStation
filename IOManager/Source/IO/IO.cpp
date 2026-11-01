#include "IO.hpp"
#include "../FileSystem/IOReq.hpp"
namespace MultiStation {

	namespace IO {

		

		ErrorCode Create(const std::string& filename, bool empty_if_exists) {
			Request req;
			Response res;
			req.operation_flags = empty_if_exists ? EMPTY_IF_EXISTS : 0;
			req.file_path = filename;


			return CreateReq(req, res);
		}

		ErrorCode Info(const std::string& filename, FileInfo& info){
			Request req;
			Response res;
			req.file_path = filename;


			ErrorCode ress = InfoReq(req, res);

			if (ress == SUCCESS) {
				info = res.info;
			}

			return ress;
		}

		ErrorCode Open(const std::string& filename, Access access, int& id){
			Request req;
			Response res;
			req.file_path = filename;
			req.access = access;

			ErrorCode ress = OpenReq(req, res);

			if (ress == SUCCESS) {
				id = res.handle;
			}

			return ress;
		}

		ErrorCode Read(int id, void* buffer, uint32_t count, uint32_t& readed, bool& eof){
			Request req;
			Response res;
			req.handle = id;
			req.buffer = buffer;
			req.count = count;
			ErrorCode ress = ReadReq(req, res);

			if (ress == SUCCESS) {
				readed = res.count;
				eof = res.eof;
			}

			return ress;
		}

		ErrorCode Write(int id, void* buffer, uint32_t count, uint32_t& writed){
			Request req;
			Response res;
			req.handle = id;
			req.buffer = buffer;
			req.count = count;
			ErrorCode ress = WriteReq(req, res);

			if (ress == SUCCESS) {
				writed = res.count;

			}

			return ress;
		}

		ErrorCode Flush(int id){
			Request req;
			Response res;
			req.handle = id;
			ErrorCode ress = FlushReq(req, res);

			return ress;
		}

		ErrorCode Seek(int id, int64_t pos, SeekMode mode, int64_t& final_pos){
			Request req;
			Response res;
			req.handle = id;
			req.pos = pos;
			req.seek_mode = mode;

			ErrorCode ress = SeekReq(req, res);

			if (ress == SUCCESS) {
				final_pos = res.pos;
			}

			return ress;
		}

		ErrorCode Tell(int id, int64_t& pos){
			Request req;
			Response res;
			req.handle = id;

			ErrorCode ress = TellReq(req, res);

			if (ress == SUCCESS) {
				pos = res.pos;
			}

			return ress;
		}

		ErrorCode Close(int id){
			Request req;
			Response res;
			req.handle = id;
			ErrorCode ress = CloseReq(req, res);
			return ress;
		}

	}

}
