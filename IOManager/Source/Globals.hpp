#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string>
#include <functional>
#include <vector>
#include <semaphore>
namespace MultiStation
{
    namespace IO {
        enum class Error
        {
            GOOD,
            OPEN_FAILED,
            CREATE_FAILED,
            REMOVE_FAILED,
            READ_FAILED,
            WRITE_FAILED,
            SEEK_FAILED ,
            TELL_FAILED ,
            CLOSE_FAILED
        };

        enum class Status
        {
            PENDING,
            COMPLETED,
            FAILED
        };

        enum class Access : uint16_t
        {
            READ = 0x0001,
            WRITE = 0x0002
        };

        inline Access operator|(Access a, Access b)
        {
            return static_cast<Access>(
                static_cast<uint16_t>(a) |
                static_cast<uint16_t>(b)
                );
        }

        inline bool HasAccess(Access value, Access flag)
        {
            return
                (static_cast<uint16_t>(value) &
                    static_cast<uint16_t>(flag)) != 0;
        }

        enum class SeekMode {
            START ,
            END   ,
            CUR
        };

        enum class Operation
        {
            GET_INFO_PATH ,
            GET_INFO_HANDLE,
            OPEN_FILE,
            CREATE_FILE,
            REMOVE_FILE,
            READ_FILE,
            WRITE_FILE,
            SEEK_FILE,
            TELL_FILE,
            CLOSE_FILE,

            READ_DIR,
            CREATE_DIR,
            REMOVE_DIR
        };







        struct DirectoryEntry
        {
            std::string name;
            std::string path;

            bool is_directory = false;
            size_t size = 0;
        };

        struct FileInfo
        {
            std::string name;
            std::string path;
            std::string extension;

            size_t size = 0;
            bool is_directory = false;
            bool exists = true;
        };

        struct Handle
        {
            Access  access = Access::READ;
            void*   native_handle = nullptr;

            std::string path;
            std::string name;
            std::string extt;
            
            
        };

        struct Result;

        using RequestCallBack = std::function<void(Result&)>;

        struct Request
        {
            Operation               operation = Operation::OPEN_FILE;

            Access                  access = Access::READ;

            std::string             path;

            void*                   read_buffer = nullptr;

            const void*             write_buffer = nullptr;

            size_t                  count = 0;

            SeekMode                mode = SeekMode::START;

            size_t                  pos = 0;

            Handle*                 handle = nullptr;

            std::binary_semaphore   semaphore{ 0 };

            bool                    is_sync = false;

            Result*                 result;

            RequestCallBack         callback;
        };
         
        struct Result
        {
            Operation       operation = Operation::OPEN_FILE;

            Status          status = Status::PENDING;

            Error           error = Error::GOOD;

            Handle*         handle = nullptr;

            size_t          transferred_bytes = 0;

            size_t          pos = 0;

            FileInfo        info;

            bool            eof = false;

            RequestCallBack callback;

            std::vector<DirectoryEntry> entries = {};
        };

        
    }
}
