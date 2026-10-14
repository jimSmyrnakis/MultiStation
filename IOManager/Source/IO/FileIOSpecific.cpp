#define _CRT_SECURE_NO_WARNINGS
#include "IOManager.hpp"

#include <stdio.h>
#include <filesystem>
#include <algorithm>
namespace MultiStation {

	namespace IO {
        namespace fs = std::filesystem;
        static const char* GetOpenMode(Access access)
        {
            const bool read = HasAccess(access, Access::READ);
            const bool write = HasAccess(access, Access::WRITE);

            if (read && write)
                return "r+b";

            if (read)
                return "rb";

            /*
             * fopen has no "existing write-only without truncation"
             * mode. r+b is used internally, while Handle::access
             * can still enforce WRITE-only semantics.
             */
            if (write)
                return "r+b";

            return nullptr;
        }


        static const char* GetCreateMode(Access access)
        {
            const bool read = HasAccess(access, Access::READ);
            const bool write = HasAccess(access, Access::WRITE);

            if (read && write)
                return "w+b";

            if (write)
                return "wb";

            /*
             * Creating a READ-only file isn't especially meaningful,
             * but w+b allows creation and the engine can expose it
             * afterwards as READ access.
             */
            if (read)
                return "w+b";

            return nullptr;
        }


        static int SeekNative(FILE* file, size_t pos, SeekMode mode)
        {
            int origin = SEEK_SET;

            switch (mode)
            {
            case SeekMode::START:
                origin = SEEK_SET;
                break;

            case SeekMode::CUR:
                origin = SEEK_CUR;
                break;

            case SeekMode::END:
                origin = SEEK_END;
                break;
            }

#if defined(_WIN32)
            return _fseeki64(
                file,
                static_cast<__int64>(pos),
                origin
            );
#else
            return fseeko(
                file,
                static_cast<off_t>(pos),
                origin
            );
#endif
        }


        static size_t TellNative(FILE* file)
        {
#if defined(_WIN32)
            const __int64 pos = _ftelli64(file);

            if (pos < 0)
                return static_cast<size_t>(-1);

            return static_cast<size_t>(pos);
#else
            const off_t pos = ftello(file);

            if (pos < 0)
                return static_cast<size_t>(-1);

            return static_cast<size_t>(pos);
#endif
        }

        bool IOManager::ExecuteRequest(Request& req, Result& res) {
            res.operation = req.operation;
            res.status = Status::PENDING;
            res.error = Error::GOOD;
            res.callback = req.callback;

            switch (req.operation)
            {
                // =====================================================
                // GET FILE INFO PATH
                // =====================================================
            case Operation::GET_INFO_PATH:
            {
                std::error_code ec;

                if (!fs::exists(req.path, ec) || ec)
                {
                    res.status = Status::FAILED;
                    res.error = Error::READ_FAILED;
                    break;
                }

                FileInfo info;

                fs::path path(req.path);

                info.exists = true;
                info.path = path.string();
                info.name = path.filename().string();
                info.extension = path.extension().string();
                info.is_directory = fs::is_directory(path, ec);

                if (ec)
                {
                    res.status = Status::FAILED;
                    res.error = Error::READ_FAILED;
                    break;
                }

                if (!info.is_directory)
                {
                    info.size =
                        static_cast<size_t>(
                            fs::file_size(path, ec)
                            );

                    if (ec)
                    {
                        res.status = Status::FAILED;
                        res.error = Error::READ_FAILED;
                        break;
                    }
                }

                res.info = info;
                res.status = Status::COMPLETED;

                break;
            }

                // =====================================================
                // GET FILE INFO HANDLE
                // =====================================================
            case Operation::GET_INFO_HANDLE:
            {
                if (
                    !req.handle ||
                    !req.handle->native_handle
                    )
                {
                    res.status = Status::FAILED;
                    res.error = Error::READ_FAILED;
                    break;
                }

                FILE* file =
                    static_cast<FILE*>(
                        req.handle->native_handle
                        );

                const size_t currentPos =
                    TellNative(file);

                if (
                    currentPos ==
                    static_cast<size_t>(-1)
                    )
                {
                    res.status = Status::FAILED;
                    res.error = Error::TELL_FAILED;
                    break;
                }

                if (
                    SeekNative(
                        file,
                        0,
                        SeekMode::END
                    ) != 0
                    )
                {
                    res.status = Status::FAILED;
                    res.error = Error::SEEK_FAILED;
                    break;
                }

                const size_t fileSize =
                    TellNative(file);

                if (
                    fileSize ==
                    static_cast<size_t>(-1)
                    )
                {
                    res.status = Status::FAILED;
                    res.error = Error::TELL_FAILED;
                    break;
                }

                if (
                    SeekNative(
                        file,
                        currentPos,
                        SeekMode::START
                    ) != 0
                    )
                {
                    res.status = Status::FAILED;
                    res.error = Error::SEEK_FAILED;
                    break;
                }

                res.info.exists = true;
                res.info.size = fileSize;
                res.info.path = res.handle->path;
                res.info.extension = res.handle->extt;
                res.info.name = res.handle->name;
                res.info.is_directory = false;

                res.handle = req.handle;
                res.status = Status::COMPLETED;

                break;
            }
                // =====================================================
                // OPEN FILE
                // =====================================================

            case Operation::OPEN_FILE:
            {
                const char* mode = GetOpenMode(req.access);

                if (!mode)
                {
                    res.status = Status::FAILED;
                    res.error = Error::OPEN_FAILED;
                    break;
                }

                FILE* file =
                    fopen(req.path.c_str(), mode);

                if (!file)
                {
                    res.status = Status::FAILED;
                    res.error = Error::OPEN_FAILED;
                    break;
                }

                Handle* handle = new Handle();

                handle->access = req.access;
                handle->native_handle = file;
                fs::path path(req.path);

                handle->path = path.string();
                handle->name = path.filename().string();
                handle->extt = path.extension().string();

                m_handels.push_back(handle);

                res.handle = handle;
                res.status = Status::COMPLETED;

                break;
            }


            // =====================================================
            // CREATE FILE
            // =====================================================

            case Operation::CREATE_FILE:
            {
                const char* mode = GetCreateMode(req.access);

                if (!mode)
                {
                    res.status = Status::FAILED;
                    res.error = Error::CREATE_FAILED;
                    break;
                }

                FILE* file =
                    fopen(req.path.c_str(), mode);

                if (!file)
                {
                    res.status = Status::FAILED;
                    res.error = Error::CREATE_FAILED;
                    break;
                }

                Handle* handle = new Handle();

                handle->access = req.access;
                handle->native_handle = file;
                fs::path path(req.path);

                handle->path = path.string();
                handle->name = path.filename().string();
                handle->extt = path.extension().string();

                m_handels.push_back(handle);

                res.handle = handle;
                res.status = Status::COMPLETED;

                break;
            }


            // =====================================================
            // REMOVE FILE
            // =====================================================

            case Operation::REMOVE_FILE:
            {
                std::error_code ec;

                const bool removed =
                    fs::remove(req.path, ec); 

                if (!removed || ec)
                {
                    res.status = Status::FAILED;
                    res.error = Error::REMOVE_FAILED;
                    break;
                }

                res.status = Status::COMPLETED;

                break;
            }


            // =====================================================
            // READ FILE
            // =====================================================

            case Operation::READ_FILE:
            {
                if (!HasAccess(
                    req.handle->access,
                    Access::READ))
                {
                    res.status = Status::FAILED;
                    res.error = Error::READ_FAILED;
                    break;
                }

                FILE* file =
                    static_cast<FILE*>(
                        req.handle->native_handle
                        );

                const size_t bytesRead =
                    fread(
                        req.read_buffer,
                        1,
                        req.count,
                        file
                    );

                res.handle = req.handle;
                res.transferred_bytes = bytesRead;

                if (bytesRead < req.count &&
                    ferror(file))
                {
                    res.status = Status::FAILED;
                    res.error = Error::READ_FAILED;

                    clearerr(file);

                    break;
                }
                if (feof(file))
                    res.eof = true;
                /*
                 * bytesRead < count + EOF is not necessarily
                 * an error. It simply means EOF was reached.
                 */
                res.status = Status::COMPLETED;

                break;
            }


            // =====================================================
            // WRITE FILE
            // =====================================================

            case Operation::WRITE_FILE:
            {
                if (!HasAccess(
                    req.handle->access,
                    Access::WRITE))
                {
                    res.status = Status::FAILED;
                    res.error = Error::WRITE_FAILED;
                    break;
                }

                FILE* file =
                    static_cast<FILE*>(
                        req.handle->native_handle
                        );

                const size_t bytesWritten =
                    fwrite(
                        req.write_buffer,
                        1,
                        req.count,
                        file
                    );

                res.handle = req.handle;
                res.transferred_bytes = bytesWritten;

                if (bytesWritten != req.count)
                {
                    res.status = Status::FAILED;
                    res.error = Error::WRITE_FAILED;
                    break;
                }

                res.status = Status::COMPLETED;

                break;
            }


            // =====================================================
            // SEEK FILE
            // =====================================================

            case Operation::SEEK_FILE:
            {
                FILE* file =
                    static_cast<FILE*>(
                        req.handle->native_handle
                        );

                if (SeekNative(
                    file,
                    req.pos,
                    req.mode) != 0)
                {
                    res.status = Status::FAILED;
                    res.error = Error::SEEK_FAILED;
                    break;
                }

                const size_t pos = TellNative(file);

                if (pos == static_cast<size_t>(-1))
                {
                    res.status = Status::FAILED;
                    res.error = Error::SEEK_FAILED;
                    break;
                }

                res.handle = req.handle;
                res.pos = pos;
                res.status = Status::COMPLETED;

                break;
            }


            // =====================================================
            // TELL FILE
            // =====================================================

            case Operation::TELL_FILE:
            {
                FILE* file =
                    static_cast<FILE*>(
                        req.handle->native_handle
                        );

                const size_t pos =
                    TellNative(file);

                if (pos == static_cast<size_t>(-1))
                {
                    res.status = Status::FAILED;
                    res.error = Error::TELL_FAILED;
                    break;
                }

                res.handle = req.handle;
                res.pos = pos;
                res.status = Status::COMPLETED;

                break;
            }


            // =====================================================
            // CLOSE FILE
            // =====================================================

            case Operation::CLOSE_FILE:
            {
                FILE* file =
                    static_cast<FILE*>(
                        req.handle->native_handle
                        );

                const int closeResult =
                    fclose(file);

                req.handle->native_handle = nullptr;

                auto it =
                    std::find(
                        m_handels.begin(),
                        m_handels.end(),
                        req.handle
                    );

                if (it != m_handels.end())
                    m_handels.erase(it);

                /*
                 * fclose invalidates FILE* even if flushing
                 * produced an error, so the engine Handle
                 * should no longer be used.
                 */
                delete req.handle;

                res.handle = nullptr;

                if (closeResult != 0)
                {
                    res.status = Status::FAILED;
                    res.error = Error::CLOSE_FAILED;
                    break;
                }

                res.status = Status::COMPLETED;

                break;
            }


            // =====================================================
            // READ DIRECTORY
            // =====================================================

            case Operation::READ_DIR:
            {
                std::error_code ec;

                if (!fs::is_directory(req.path, ec) || ec)
                {
                    res.status = Status::FAILED;
                    res.error = Error::READ_FAILED;
                    break;
                }

                for (const auto& entry :
                    fs::directory_iterator(req.path, ec))
                {
                    if (ec)
                        break;

                    DirectoryEntry dirEntry;

                    dirEntry.name =
                        entry.path()
                        .filename()
                        .string();

                    dirEntry.path =
                        entry.path().string();

                    dirEntry.is_directory =
                        entry.is_directory(ec);

                    if (ec)
                        break;

                    if (!dirEntry.is_directory)
                    {
                        dirEntry.size =
                            static_cast<size_t>(
                                entry.file_size(ec)
                                );

                        if (ec)
                        {
                            ec.clear();
                            dirEntry.size = 0;
                        }
                    }

                    res.entries.push_back(
                        std::move(dirEntry)
                    );
                }

                if (ec)
                {
                    res.entries.clear();

                    res.status = Status::FAILED;
                    res.error = Error::READ_FAILED;

                    break;
                }

                res.status = Status::COMPLETED;

                break;
            }


            // =====================================================
            // CREATE DIRECTORY
            // =====================================================

            case Operation::CREATE_DIR:
            {
                std::error_code ec;

                /*
                 * create_directory() returns false when the
                 * directory already exists, which we don't
                 * necessarily want to treat as an IO failure.
                 */
                if (fs::exists(req.path, ec))
                {
                    if (ec ||
                        !fs::is_directory(req.path, ec))
                    {
                        res.status = Status::FAILED;
                        res.error = Error::CREATE_FAILED;
                        break;
                    }

                    res.status = Status::COMPLETED;
                    break;
                }

                if (!fs::create_directory(req.path, ec) ||
                    ec)
                {
                    res.status = Status::FAILED;
                    res.error = Error::CREATE_FAILED;
                    break;
                }

                res.status = Status::COMPLETED;

                break;
            }


            // =====================================================
            // REMOVE DIRECTORY
            // =====================================================

            case Operation::REMOVE_DIR:
            {
                std::error_code ec;

                /*
                 * fs::remove removes an empty directory.
                 * It does NOT recursively delete its contents.
                 */
                const bool removed =
                    fs::remove(req.path, ec);

                if (!removed || ec)
                {
                    res.status = Status::FAILED;
                    res.error = Error::REMOVE_FAILED;
                    break;
                }

                res.status = Status::COMPLETED;

                break;
            }


            default:
            {
                res.status = Status::FAILED;
                break;
            }
            }
        }

	}

}
