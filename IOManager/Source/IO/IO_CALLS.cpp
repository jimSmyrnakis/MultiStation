#include "IO.hpp"

namespace MultiStation {

	namespace IO {
        static Request* CreateRequest()
        {
            return new (std::nothrow) Request;
        }

        static Result* CreateResult()
        {
            return new (std::nothrow) Result;
        }

        static void DestroyRequest(Request* request)
        {
            delete request;
        }

        static void DestroyResult(Result* result)
        {
            delete result;
        }

        // =========================================================
// Sync File API
// =========================================================

        Handle* OpenFileSync(
            const std::string& path,
            Access access)
        {
            Request* request = CreateRequest();
            Result* result = CreateResult();

            if (!request || !result)
            {
                DestroyRequest(request);
                DestroyResult(result);
                return nullptr;
            }

            request->operation = Operation::OPEN_FILE;
            request->path = path;
            request->access = access;

            if (!s_manager.ExecuteSync(request, result))
            {
                DestroyRequest(request);
                DestroyResult(result);
                return nullptr;
            }

            Handle* handle = nullptr;

            if (result->status == Status::COMPLETED)
                handle = result->handle;

            DestroyRequest(request);
            DestroyResult(result);

            return handle;
        }


        Handle* CreateFileSync(
            const std::string& path,
            Access access)
        {
            Request* request = CreateRequest();
            Result* result = CreateResult();

            if (!request || !result)
            {
                DestroyRequest(request);
                DestroyResult(result);
                return nullptr;
            }

            request->operation = Operation::CREATE_FILE;
            request->path = path;
            request->access = access;

            if (!s_manager.ExecuteSync(request, result))
            {
                DestroyRequest(request);
                DestroyResult(result);
                return nullptr;
            }

            Handle* handle = nullptr;

            if (result->status == Status::COMPLETED)
                handle = result->handle;

            DestroyRequest(request);
            DestroyResult(result);

            return handle;
        }


        bool RemoveFileSync(
            const std::string& path)
        {
            Request* request = CreateRequest();
            Result* result = CreateResult();

            if (!request || !result)
            {
                DestroyRequest(request);
                DestroyResult(result);
                return false;
            }

            request->operation = Operation::REMOVE_FILE;
            request->path = path;

            bool success =
                s_manager.ExecuteSync(request, result) &&
                result->status == Status::COMPLETED;

            DestroyRequest(request);
            DestroyResult(result);

            return success;
        }


        size_t ReadFileSync(
            Handle* handle,
            void* buffer,
            size_t count, 
            bool& eof)
        {
            eof = false;
            if (!handle || !buffer)
                return 0;

            Request* request = CreateRequest();
            Result* result = CreateResult();

            if (!request || !result)
            {
                DestroyRequest(request);
                DestroyResult(result);
                return 0;
            }

            request->operation = Operation::READ_FILE;
            request->handle = handle;
            request->read_buffer = buffer;
            request->count = count;

            size_t transferred = 0;
            
            if (s_manager.ExecuteSync(request, result) &&
                result->status == Status::COMPLETED)
            {
                eof = result->eof;
                transferred = result->transferred_bytes;
            }

            DestroyRequest(request);
            DestroyResult(result);

            return transferred;
        }


        size_t SeekFileSync(
            Handle* handle,
            size_t pos,
            SeekMode mode)
        {
            if (!handle)
                return 0;

            Request* request = CreateRequest();
            Result* result = CreateResult();

            if (!request || !result)
            {
                DestroyRequest(request);
                DestroyResult(result);
                return 0;
            }

            request->operation = Operation::SEEK_FILE;
            request->handle = handle;
            request->pos = pos;
            request->mode = mode;

            size_t result_pos = 0;

            if (s_manager.ExecuteSync(request, result) &&
                result->status == Status::COMPLETED)
            {
                result_pos = result->pos;
            }

            DestroyRequest(request);
            DestroyResult(result);

            return result_pos;
        }


        size_t TellFileSync(
            Handle* handle)
        {
            if (!handle)
                return 0;

            Request* request = CreateRequest();
            Result* result = CreateResult();

            if (!request || !result)
            {
                DestroyRequest(request);
                DestroyResult(result);
                return 0;
            }

            request->operation = Operation::TELL_FILE;
            request->handle = handle;

            size_t pos = 0;

            if (s_manager.ExecuteSync(request, result) &&
                result->status == Status::COMPLETED)
            {
                pos = result->pos;
            }

            DestroyRequest(request);
            DestroyResult(result);

            return pos;
        }


        size_t WriteFileSync(
            Handle* handle,
            const void* buffer,
            size_t count)
        {
            if (!handle || !buffer)
                return 0;

            Request* request = CreateRequest();
            Result* result = CreateResult();

            if (!request || !result)
            {
                DestroyRequest(request);
                DestroyResult(result);
                return 0;
            }

            request->operation = Operation::WRITE_FILE;
            request->handle = handle;
            request->write_buffer = buffer;
            request->count = count;

            size_t transferred = 0;

            if (s_manager.ExecuteSync(request, result) &&
                result->status == Status::COMPLETED)
            {
                transferred = result->transferred_bytes;
            }

            DestroyRequest(request);
            DestroyResult(result);

            return transferred;
        }


        bool CloseFileSync(
            Handle* handle)
        {
            if (!handle)
                return false;

            Request* request = CreateRequest();
            Result* result = CreateResult();

            if (!request || !result)
            {
                DestroyRequest(request);
                DestroyResult(result);
                return false;
            }

            request->operation = Operation::CLOSE_FILE;
            request->handle = handle;

            bool success =
                s_manager.ExecuteSync(request, result) &&
                result->status == Status::COMPLETED;

            DestroyRequest(request);
            DestroyResult(result);

            return success;
        }


        // =========================================================
        // Sync Directory API
        // =========================================================

        bool ReadDirSync(
            const std::string& path,
            std::vector<DirectoryEntry>& entries)
        {
            Request* request = CreateRequest();
            Result* result = CreateResult();

            if (!request || !result)
            {
                DestroyRequest(request);
                DestroyResult(result);
                return false;
            }

            request->operation = Operation::READ_DIR;
            request->path = path;

            bool success =
                s_manager.ExecuteSync(request, result) &&
                result->status == Status::COMPLETED;

            if (success)
                entries = std::move(result->entries);

            DestroyRequest(request);
            DestroyResult(result);

            return success;
        }


        bool CreateDirSync(
            const std::string& path)
        {
            Request* request = CreateRequest();
            Result* result = CreateResult();

            if (!request || !result)
            {
                DestroyRequest(request);
                DestroyResult(result);
                return false;
            }

            request->operation = Operation::CREATE_DIR;
            request->path = path;

            bool success =
                s_manager.ExecuteSync(request, result) &&
                result->status == Status::COMPLETED;

            DestroyRequest(request);
            DestroyResult(result);

            return success;
        }


        bool RemoveDirSync(
            const std::string& path)
        {
            Request* request = CreateRequest();
            Result* result = CreateResult();

            if (!request || !result)
            {
                DestroyRequest(request);
                DestroyResult(result);
                return false;
            }

            request->operation = Operation::REMOVE_DIR;
            request->path = path;

            bool success =
                s_manager.ExecuteSync(request, result) &&
                result->status == Status::COMPLETED;

            DestroyRequest(request);
            DestroyResult(result);

            return success;
        }

        bool GetFileInfoSync(
            const std::string& path,
            FileInfo& info)
        {
            Request* request = CreateRequest();
            Result* result = CreateResult();

            if (!request || !result)
            {
                DestroyRequest(request);
                DestroyResult(result);
                return false;
            }

            request->operation = Operation::GET_INFO_PATH;
            request->path = path;


            bool success =
                s_manager.ExecuteSync(request, result) &&
                result->status == Status::COMPLETED;
            if (success) {
                info = result->info;
            }
            else {
                info.exists = false;
            }
            DestroyRequest(request);
            DestroyResult(result);

            return success;
        }

        bool GetFileInfoSync(Handle* handle, FileInfo& info)
        {
            if (!handle)
                return false;

            Request* request = CreateRequest();
            Result* result = CreateResult();

            if (!request || !result)
            {
                DestroyRequest(request);
                DestroyResult(result);
                return false;
            }

            request->operation = Operation::GET_INFO_HANDLE;
            request->handle = handle;

            bool success =
                s_manager.ExecuteSync(request, result) &&
                result->status == Status::COMPLETED;
            if (success) {
                info = result->info;
            }
            else {
                info.exists = false;
            }

            DestroyRequest(request);
            DestroyResult(result);

            return success;
        }










        // =========================================================
// Async File API
// =========================================================

        bool OpenFileAsync(
            const std::string& path,
            Access access,
            RequestCallBack callback)
        {
            Request* request = CreateRequest();

            if (!request)
                return false;

            request->operation = Operation::OPEN_FILE;
            request->path = path;
            request->access = access;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                DestroyRequest(request);
                return false;
            }

            return true;
        }


        bool CreateFileAsync(
            const std::string& path,
            Access access,
            RequestCallBack callback)
        {
            Request* request = CreateRequest();

            if (!request)
                return false;

            request->operation = Operation::CREATE_FILE;
            request->path = path;
            request->access = access;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                DestroyRequest(request);
                return false;
            }

            return true;
        }


        bool RemoveFileAsync(
            const std::string& path,
            RequestCallBack callback)
        {
            Request* request = CreateRequest();

            if (!request)
                return false;

            request->operation = Operation::REMOVE_FILE;
            request->path = path;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                DestroyRequest(request);
                return false;
            }

            return true;
        }


        bool ReadFileAsync(
            Handle* handle,
            void* buffer,
            size_t count,
            RequestCallBack callback)
        {
            Request* request = CreateRequest();

            if (!request)
                return false;

            request->operation = Operation::READ_FILE;
            request->handle = handle;
            request->read_buffer = buffer;
            request->count = count;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                DestroyRequest(request);
                return false;
            }

            return true;
        }


        bool SeekFileAsync(
            Handle* handle,
            size_t pos,
            SeekMode mode,
            RequestCallBack callback)
        {
            Request* request = CreateRequest();

            if (!request)
                return false;

            request->operation = Operation::SEEK_FILE;
            request->handle = handle;
            request->pos = pos;
            request->mode = mode;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                DestroyRequest(request);
                return false;
            }

            return true;
        }


        bool TellFileAsync(
            Handle* handle,
            RequestCallBack callback)
        {
            Request* request = CreateRequest();

            if (!request)
                return false;

            request->operation = Operation::TELL_FILE;
            request->handle = handle;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                DestroyRequest(request);
                return false;
            }

            return true;
        }


        bool WriteFileAsync(
            Handle* handle,
            const void* buffer,
            size_t count,
            RequestCallBack callback)
        {
            Request* request = CreateRequest();

            if (!request)
                return false;

            request->operation = Operation::WRITE_FILE;
            request->handle = handle;
            request->write_buffer = buffer;
            request->count = count;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                DestroyRequest(request);
                return false;
            }

            return true;
        }


        bool CloseFileAsync(
            Handle* handle,
            RequestCallBack callback)
        {
            Request* request = CreateRequest();

            if (!request)
                return false;

            request->operation = Operation::CLOSE_FILE;
            request->handle = handle;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                DestroyRequest(request);
                return false;
            }

            return true;
        }


        // =========================================================
        // Async Directory API
        // =========================================================

        bool ReadDirAsync(
            const std::string& path,
            RequestCallBack callback)
        {
            Request* request = CreateRequest();

            if (!request)
                return false;

            request->operation = Operation::READ_DIR;
            request->path = path;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                DestroyRequest(request);
                return false;
            }

            return true;
        }


        bool CreateDirAsync(
            const std::string& path,
            RequestCallBack callback)
        {
            Request* request = CreateRequest();

            if (!request)
                return false;

            request->operation = Operation::CREATE_DIR;
            request->path = path;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                DestroyRequest(request);
                return false;
            }

            return true;
        }


        bool RemoveDirAsync(
            const std::string& path,
            RequestCallBack callback)
        {
            Request* request = CreateRequest();

            if (!request)
                return false;

            request->operation = Operation::REMOVE_DIR;
            request->path = path;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                DestroyRequest(request);
                return false;
            }

            return true;
        }

        bool GetFileInfoAsync(
            const std::string& path,
            RequestCallBack callback)
        {
            Request* request = CreateRequest();

            if (!request)
                return false;

            request->operation = Operation::GET_INFO_PATH;
            request->path = path;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                DestroyRequest(request);
                return false;
            }

            return true;
        }


        bool GetFileInfoAsync(
            Handle* handle,
            RequestCallBack callback)
        {
            if (!handle)
                return false;

            Request* request = CreateRequest();

            if (!request)
                return false;

            request->operation = Operation::GET_INFO_HANDLE;
            request->handle = handle;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                DestroyRequest(request);
                return false;
            }

            return true;
        }

	}

}
