#include "IOManager.hpp"


namespace MultiStation
{
    namespace IO
    {
        


        static bool IsReqGood(const Request& req)
        {
            switch (req.operation)
            {
            case Operation::OPEN_FILE:
            case Operation::CREATE_FILE:
                return !req.path.empty();

            case Operation::REMOVE_FILE:
            case Operation::READ_DIR:
            case Operation::CREATE_DIR:
            case Operation::REMOVE_DIR:
                return !req.path.empty();

            case Operation::READ_FILE:
                return req.handle != nullptr &&
                    req.handle->native_handle != nullptr &&
                    req.read_buffer != nullptr &&
                    req.count > 0;

            case Operation::WRITE_FILE:
                return req.handle != nullptr &&
                    req.handle->native_handle != nullptr &&
                    req.write_buffer != nullptr &&
                    req.count > 0;

            case Operation::SEEK_FILE:
            case Operation::TELL_FILE:
            case Operation::CLOSE_FILE:
                return req.handle != nullptr &&
                    req.handle->native_handle != nullptr;

            default:
                return false;
            }
        }


        
        bool IOManager::ExecuteSync(Request* reqio, Result* resio) {
            if (!reqio || !resio)
                return false;

            if (!IsReqGood(*reqio))
                return false;

            reqio->is_sync = true;
            reqio->result = resio;
            m_requestsQueue.Push(reqio);

            reqio->semaphore.acquire();
            reqio->result = nullptr;
            
            return true;
        }

        bool IOManager::PushRequest(Request* reqio)
        {

            if (!reqio || !IsReqGood(*reqio))
                return false;
            reqio->is_sync = false;
            reqio->result = nullptr;
            m_requestsQueue.Push(reqio);

            return true;
        }


        bool IOManager::PopResult(
            Result*& resio)
        {
            return m_resultsQueue.Pop(resio);
        }

        
        
        void IOManager::UpdateIO()
        {
            Request* req = nullptr;

            if (!m_requestsQueue.Pop(req))
                return;

            if (!req)
                return;

            if (req->is_sync) // sync
            {
                if (!req->result)
                    return;
                ExecuteRequest(*req, *req->result);

                req->semaphore.release();
            }
            else // async
            {
                Result* res = new (std::nothrow) Result;

                if (!res)
                    return;

                ExecuteRequest(*req, *res);

                m_resultsQueue.Push(res);
            }
        }

        void IOManager::Fini(void) {
            // while request's exists
            // do them before close
            while (m_requestsQueue.Size() != 0) {
                UpdateIO();
            }

        }
    }
}
