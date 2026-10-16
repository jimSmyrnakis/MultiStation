#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string>
#include <functional>
#include <vector>
#include <semaphore>
#include <memory>
/**
 * @author Dimitris Smyrnakis 
 * @file Globals.hpp 
 * @brief Contains all necessery structs and types used for io manager for synchronous and asynchronous
 * operations , as whell the definition of the asynchronous callback . 
 */
namespace MultiStation
{
    namespace IO {

        /**
         * @enum Error  
         * @brief Contains IO Errors for io operations 
         * for moment i have the simplest's ones 
         */
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

        /**
         * @enum Status 
         * @brief Contains the status choisses used for request state
         */
        enum class Status
        {
            PENDING,
            COMPLETED,
            FAILED
        };

        /**
         * @enum Access 
         * @brief Contains the access mode for opening files .
         */
        enum class Access : uint16_t
        {
            READ = 0x0001,
            WRITE = 0x0002
        };

        Access operator|(Access a, Access b);

        bool HasAccess(Access value, Access flag);

        /**
         * @enum SeekMode
         * @brief Used from the seek io methods for setting up
         * the offset beggining , START => from the beggining , 
         * END from the end , CUR from the current position .
         */
        enum class SeekMode {
            START ,
            END   ,
            CUR
        };

        /**
         * @enum Operation
         * @brief Contains all operation types , like open , close , read , write 
         * files , or directory operations etc. 
         */
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






        /**
         * @struct DirectoryEntry
         * @brief Represents one entry info from a directory .
         */
        struct DirectoryEntry
        {
            /**
             * @var name
             * @brief The name of the entry
             */
            std::string name;
            /**
             * @var path
             * @brief The path to this entry
             */
            std::string path;
            /**
             * @var parent
             * @brief parent path 
             */
            std::string parent;
            /**
             * @var is_directory
             * @brief True if is directory , false if is file . 
             */
            bool is_directory = false;
            /**
             * @var size 
             * @brief Size in bytes of this entry , if is a file .
             */
            size_t size = 0;
        };

        /**
         * @struct FileInfo 
         * @brief Describes basic info of a file , like name , path 
         * , extension , size in bytes or if the file requested for info exists or
         * not .
         */
        struct FileInfo
        {
            /**
             * @var name
             * @brief File name 
             */
            std::string name;
            /**
             * @var path
             * @brief File path
             */
            std::string path;
            /**
             * @var extension
             * @brief File extension if is a file and not a folder
             */
            std::string extension;

            /**
             * @var size
             * @brief If is file then its size in bytes
             */
            size_t size = 0;
            /**
             * @var is_directory
             * @brief true if is directory 
             */
            bool is_directory = false;
            /**
             * @var exists
             * @brief true if the requested file/folder path exists
             */
            bool exists = false;
        };

        /**
         * @struct Handle
         * @brief This is the struct represented the handle to a file for io operations after openning 
         * a file .
         */
        struct Handle
        {
            /**
             * @var access
             * @brief The Access mode of the file
             */
            Access  access = Access::READ;
            /**
             * @var native_handler
             * @brief The os specific file handler 
             */
            void*   native_handle = nullptr;

            /**
             * @var name
             * @brief File name
             */
            std::string name;
            /**
             * @var path
             * @brief File path
             */
            std::string path;
            /**
             * @var extt
             * @brief File extension if is a file and not a folder
             */
            std::string extt;
            
            
        };

        struct Result;

        /**
         * @typedef RequestCallBack
         * @brief The method definition lamda for io async callback 
         * parsing through the Result reference . 
         */
        using RequestCallBack = std::function<void(Result&)>;

        /**
         * @struct Request 
         * @brief Contains many private and public filed's for use , privates
         * used from io manager and public ones used by the IO methods 
         */
        struct Request
        {
            /**
             * @var operation 
             * @brief The io operation to perfom  
             */
            Operation               operation = Operation::OPEN_FILE;
            /**
             * @var access
             * @brief The access mode for open file operations 
             */
            Access                  access = Access::READ;
            /**
             * @var path
             * @brief The file/folder path
             */
            std::string             path;
            /**
             * @var read_buffer
             * @brief The buffer to write data from file
             * for read operations 
             */
            void*                   read_buffer = nullptr;
            /**
             * @var write_buffer
             * @brief The buffer to write data to a file
             * for write operations
             */
            const void*             write_buffer = nullptr;
            /**
             * @var count
             * @brief Count in bytes of the read/write buffer depending the 
             * operation
             */
            size_t                  count = 0;
            /**
             * @var mode 
             * @brief Used for seek operations and is from where pos speaks relative to .
             */
            SeekMode                mode = SeekMode::START;
            /**
             * @var pos 
             * @brief Used by tell , seek operations and gives the position in bytes inside 
             * a file from where next read/write operation will happen .
             */
            size_t                  pos = 0;
            /**
             * @var handle
             * @brief pointer to a file handle , this is created and used from io manager for 
             * the specific file 
             */
            std::shared_ptr<Handle> handle = nullptr;
            /**
             * @var semaphore 
             * @brief Used and initiallized only by the io manager for synchronous io 
             */
            std::binary_semaphore   semaphore{ 0 };
            /**
             * @var is_sync
             * @brief true if the request is for sync , false otherwise . Still used only by the 
             * io manager 
             */
            bool                    is_sync = false;
            /**
             * @var result
             * @brief Used and initiallized by the io manager  
             */
            std::shared_ptr<Result> result;
            /**
             * @var callback
             * @brief This is given from user (still IO hides that pass info) 
             * for the io manager to pass the callback back two the result .
             */
            RequestCallBack         callback;
        };
         
        struct Result
        {
            Operation                   operation = Operation::OPEN_FILE;

            Status                      status = Status::PENDING;

            Error                       error = Error::GOOD;

            std::shared_ptr<Handle>     handle = nullptr;

            size_t                      transferred_bytes = 0;

            size_t                      pos = 0;

            FileInfo                    info;

            bool                        eof = false;

            RequestCallBack             callback;

            std::vector<DirectoryEntry> entries = {};
        };

        
    }
}
