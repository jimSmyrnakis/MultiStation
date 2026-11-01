#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include <stddef.h>
#include <string>
#include <functional>
#include <vector>
#include <semaphore>
#include <memory>
#include <Platform.hpp>
#include "DirectoryEntry.hpp"
/**
 * @author Dimitris Smyrnakis 
 * @file Globals.hpp 
 * @brief Contains all necessery structs and types used for io manager for synchronous and asynchronous
 * operations , as whell the definition of the asynchronous callback . 
 * @version 0.1
 */
namespace MultiStation
{
    namespace IO {

        

        enum class Access : uint16_t {
            READ = 0x0001,
            WRITE = 0x0002,
            APPEND = 0x0004
            
        };
        bool operator==(Access& ths , Access& other);
        bool IsEqual(Access a, Access b);
        Access operator|(Access a, Access b);
        bool HasAccess(Access value, Access flag);


        enum class Operations {
            OPEN    , 
            CREATE  ,
            CLOSE   ,

            READ    ,
            SEEK    ,
            TELL    ,
            WRITE   ,
            FLUSH   ,

            INFO    ,

            READ_DIR,
            RMVE_DIR,
            MAKE_DIR
            
        };

        enum class SeekMode {
            END,
            START,
            CURRENT
        };

        struct Response;
        using Callback = std::function<void(Response)>;

        struct Request {

            // defines the operation that must be done
            Operations  operation;

            // Access mode for create , Open operations
            Access      access;

            // Used by the opearation io interface for specific behaviors on operations 
            // io implementations
            uint32_t    operation_flags;

            // File path , for operations like Open/Create
            // Read/Remove/Make dir .
            std::string file_path;

            // buffer for read / write operations
            void*       buffer;

            // count buffer bytes
            uint32_t    count;

            // seek mode used for seek operation
            SeekMode    seek_mode;

            // pos relative to seek mode , used for seek operation
            int64_t     pos;

            // callback for response call after request is completed
            Callback    callback;

            // handle id
            int         handle;

            


        };

        
        struct Response {
            // defines the operation that must be done
            Operations  operation;

            // give us if the operation failed or not and may give why
            ErrorCode   error;

            // count buffer bytes for read/write operation
            uint32_t    count;

            // pos relative to file start , is for tell operation
            int64_t     pos;

            // For directory based operations (READ/MAKE/REMOVE DIR)
            Directory   dir;

            // For file specific info operation
            FileInfo    info;

            // our given callback
            Callback    callback;

            // handle id for operations like open 
            int         handle;
            // End Of File for read operations
            bool        eof;

        };

        
    }
}
