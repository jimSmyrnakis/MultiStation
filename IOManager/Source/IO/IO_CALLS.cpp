#include "IO.hpp"

namespace MultiStation {

	namespace IO {
        

        #define s_manager IOManager::Get()

        // =========================================================
        // Sync File API
        // =========================================================

        std::shared_ptr<Handle> OpenFileSync(
            const std::string& path,
            Access access)
        {
            std::shared_ptr<Request> request = std::make_shared<Request>();
            std::shared_ptr<Result> result = std::make_shared<Result>();

            if (!request || !result)
            {
                
                return nullptr;
            }

            request->operation = Operation::OPEN_FILE;
            request->path = path;
            request->access = access;

            if (!s_manager.ExecuteSync(request, result))
            {
                
                return nullptr;
            }

            std::shared_ptr<Handle> handle = nullptr;

            if (result->status == Status::COMPLETED)
                handle = result->handle;

            

            return handle;
        }


        std::shared_ptr<Handle> CreateFileSync(
            const std::string& path,
            Access access)
        {
            std::shared_ptr<Request> request = std::make_shared<Request>();
            std::shared_ptr<Result> result = std::make_shared<Result>();

            if (!request || !result)
            {
                return nullptr;
            }

            request->operation = Operation::CREATE_FILE;
            request->path = path;
            request->access = access;

            if (!s_manager.ExecuteSync(request, result))
            {
                return nullptr;
            }

            std::shared_ptr<Handle> handle = nullptr;

            if (result->status == Status::COMPLETED)
                handle = result->handle;


            return handle;
        }


        bool RemoveFileSync(
            const std::string& path)
        {
            std::shared_ptr<Request> request = std::make_shared<Request>();
            std::shared_ptr<Result> result = std::make_shared<Result>();

            if (!request || !result)
            {
                return false;
            }

            request->operation = Operation::REMOVE_FILE;
            request->path = path;

            bool success =
                s_manager.ExecuteSync(request, result) &&
                result->status == Status::COMPLETED;


            return success;
        }


        bool ReadFileSync(
            std::shared_ptr<Handle> handle,
            void* buffer,
            size_t& count, 
            bool& eof)
        {
            eof = false;
            if (!handle || !buffer)
                return false;

            std::shared_ptr<Request> request = std::make_shared<Request>();
            std::shared_ptr<Result> result = std::make_shared<Result>();

            if (!request || !result)
            {
                return false;
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
            count = transferred;

            return true;
        }


        bool SeekFileSync(
            std::shared_ptr<Handle> handle,
            size_t& pos,
            SeekMode mode)
        {
            if (!handle)
                return false;

            std::shared_ptr<Request> request = std::make_shared<Request>();
            std::shared_ptr<Result> result = std::make_shared<Result>();

            if (!request || !result)
            {
                return false;
            }

            request->operation = Operation::SEEK_FILE;
            request->handle = handle;
            request->pos = pos;
            request->mode = mode;

            pos = 0;

            if (s_manager.ExecuteSync(request, result) &&
                result->status == Status::COMPLETED)
            {
                pos = result->pos;
            }


            return true;
        }


        bool TellFileSync(
            std::shared_ptr<Handle> handle , size_t& pos)
        {
            if (!handle)
                return 0;

            std::shared_ptr<Request> request = std::make_shared<Request>();
            std::shared_ptr<Result> result = std::make_shared<Result>();

            if (!request || !result)
            {
                return false;
            }

            request->operation = Operation::TELL_FILE;
            request->handle = handle;

            pos = 0;

            if (s_manager.ExecuteSync(request, result) &&
                result->status == Status::COMPLETED)
            {
                pos = result->pos;
            }


            return true;
        }


        bool WriteFileSync(
            std::shared_ptr<Handle> handle,
            const void* buffer,
            size_t& count)
        {
            if (!handle || !buffer)
                return false;

            std::shared_ptr<Request> request = std::make_shared<Request>();
            std::shared_ptr<Result> result = std::make_shared<Result>();

            if (!request || !result)
            {
                return false;
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

            count = transferred;
            return true;
        }


        bool CloseFileSync(
            std::shared_ptr<Handle> handle)
        {
            if (!handle)
                return false;

            std::shared_ptr<Request> request = std::make_shared<Request>();
            std::shared_ptr<Result> result = std::make_shared<Result>();

            if (!request || !result)
            {
                return false;
            }

            request->operation = Operation::CLOSE_FILE;
            request->handle = handle;

            bool success =
                s_manager.ExecuteSync(request, result) &&
                result->status == Status::COMPLETED;


            return success;
        }


        // =========================================================
        // Sync Directory API
        // =========================================================

        bool ReadDirSync(
            const std::string& path,
            std::vector<DirectoryEntry>& entries)
        {
            std::shared_ptr<Request> request = std::make_shared<Request>();
            std::shared_ptr<Result> result = std::make_shared<Result>();

            if (!request || !result)
            {
                return false;
            }

            request->operation = Operation::READ_DIR;
            request->path = path;

            bool success =
                s_manager.ExecuteSync(request, result) &&
                result->status == Status::COMPLETED;

            if (success)
                entries = std::move(result->entries);


            return success;
        }


        bool CreateDirSync(
            const std::string& path)
        {
            std::shared_ptr<Request> request = std::make_shared<Request>();
            std::shared_ptr<Result> result = std::make_shared<Result>();

            if (!request || !result)
            {
                return false;
            }

            request->operation = Operation::CREATE_DIR;
            request->path = path;

            bool success =
                s_manager.ExecuteSync(request, result) &&
                result->status == Status::COMPLETED;


            return success;
        }


        bool RemoveDirSync(
            const std::string& path)
        {
            std::shared_ptr<Request> request = std::make_shared<Request>();
            std::shared_ptr<Result> result = std::make_shared<Result>();

            if (!request || !result)
            {
                return false;
            }

            request->operation = Operation::REMOVE_DIR;
            request->path = path;

            bool success =
                s_manager.ExecuteSync(request, result) &&
                result->status == Status::COMPLETED;


            return success;
        }

        bool GetFileInfoSync(
            const std::string& path,
            FileInfo& info)
        {
            std::shared_ptr<Request> request = std::make_shared<Request>();
            std::shared_ptr<Result> result = std::make_shared<Result>();

            if (!request || !result)
            {
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

            return success;
        }

        bool GetFileInfoSync(std::shared_ptr<Handle> handle, FileInfo& info)
        {
            if (!handle)
                return false;

            std::shared_ptr<Request> request = std::make_shared<Request>();
            std::shared_ptr<Result> result = std::make_shared<Result>();

            if (!request || !result)
            {
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
            std::shared_ptr<Request> request = std::make_shared<Request>();

            if (!request)
                return false;

            request->operation = Operation::OPEN_FILE;
            request->path = path;
            request->access = access;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                
                return false;
            }

            return true;
        }


        bool CreateFileAsync(
            const std::string& path,
            Access access,
            RequestCallBack callback)
        {
            std::shared_ptr<Request> request = std::make_shared<Request>();

            if (!request)
                return false;

            request->operation = Operation::CREATE_FILE;
            request->path = path;
            request->access = access;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                
                return false;
            }

            return true;
        }


        bool RemoveFileAsync(
            const std::string& path,
            RequestCallBack callback)
        {
            std::shared_ptr<Request> request = std::make_shared<Request>();

            if (!request)
                return false;

            request->operation = Operation::REMOVE_FILE;
            request->path = path;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                ;
                return false;
            }

            return true;
        }


        bool ReadFileAsync(
            std::shared_ptr<Handle> handle,
            void* buffer,
            size_t count,
            RequestCallBack callback)
        {
            std::shared_ptr<Request> request = std::make_shared<Request>();

            if (!request)
                return false;

            request->operation = Operation::READ_FILE;
            request->handle = handle;
            request->read_buffer = buffer;
            request->count = count;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                
                return false;
            }

            return true;
        }


        bool SeekFileAsync(
            std::shared_ptr<Handle> handle,
            size_t pos,
            SeekMode mode,
            RequestCallBack callback)
        {
            std::shared_ptr<Request> request = std::make_shared<Request>();

            if (!request)
                return false;

            request->operation = Operation::SEEK_FILE;
            request->handle = handle;
            request->pos = pos;
            request->mode = mode;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                
                return false;
            }

            return true;
        }


        bool TellFileAsync(
            std::shared_ptr<Handle> handle,
            RequestCallBack callback)
        {
            std::shared_ptr<Request> request = std::make_shared<Request>();

            if (!request)
                return false;

            request->operation = Operation::TELL_FILE;
            request->handle = handle;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                
                return false;
            }

            return true;
        }


        bool WriteFileAsync(
            std::shared_ptr<Handle> handle,
            const void* buffer,
            size_t count,
            RequestCallBack callback)
        {
            std::shared_ptr<Request> request = std::make_shared<Request>();

            if (!request)
                return false;

            request->operation = Operation::WRITE_FILE;
            request->handle = handle;
            request->write_buffer = buffer;
            request->count = count;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                
                return false;
            }

            return true;
        }


        bool CloseFileAsync(
            std::shared_ptr<Handle> handle,
            RequestCallBack callback)
        {
            std::shared_ptr<Request> request = std::make_shared<Request>();

            if (!request)
                return false;

            request->operation = Operation::CLOSE_FILE;
            request->handle = handle;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                
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
            std::shared_ptr<Request> request = std::make_shared<Request>();

            if (!request)
                return false;

            request->operation = Operation::READ_DIR;
            request->path = path;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                
                return false;
            }

            return true;
        }


        bool CreateDirAsync(
            const std::string& path,
            RequestCallBack callback)
        {
            std::shared_ptr<Request> request = std::make_shared<Request>();

            if (!request)
                return false;

            request->operation = Operation::CREATE_DIR;
            request->path = path;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                
                return false;
            }

            return true;
        }


        bool RemoveDirAsync(
            const std::string& path,
            RequestCallBack callback)
        {
            std::shared_ptr<Request> request = std::make_shared<Request>();

            if (!request)
                return false;

            request->operation = Operation::REMOVE_DIR;
            request->path = path;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                
                return false;
            }

            return true;
        }

        bool GetFileInfoAsync(
            const std::string& path,
            RequestCallBack callback)
        {
            std::shared_ptr<Request> request = std::make_shared<Request>();

            if (!request)
                return false;

            request->operation = Operation::GET_INFO_PATH;
            request->path = path;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                
                return false;
            }

            return true;
        }


        bool GetFileInfoAsync(
            std::shared_ptr<Handle> handle,
            RequestCallBack callback)
        {
            if (!handle)
                return false;

            std::shared_ptr<Request> request = std::make_shared<Request>();

            if (!request)
                return false;

            request->operation = Operation::GET_INFO_HANDLE;
            request->handle = handle;
            request->callback = std::move(callback);

            if (!s_manager.PushRequest(request))
            {
                
                return false;
            }

            return true;
        }

	}

}
