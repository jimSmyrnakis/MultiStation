#pragma once
#include <string>
#include "../Globals.hpp"
#include "IOManager.hpp"
namespace MultiStation{

	namespace IO {

		/**
		 * @brief Initialize the io thread and the io manager , in other words
		 * initialize the IO .
		 */
		bool Init(void);

		/**
		 * @brief Finalize / Terminates the IO
		 */
		void Fini(void);

		/**
		 * @brief Recommend to call this function from main thread loop so operations 
		 * in callbacks are in sync with the loop .
		 */
		void PollCallbacks(void);




        /**
         * @brief Opens an existing file synchronously.
         *
         * @param path Path to the file.
         * @param access Requested access mode.
         * @return A valid file handle on success, otherwise nullptr.
         */
        std::shared_ptr<Handle> OpenFileSync(const std::string& path, Access access);

        /**
         * @brief Creates a file synchronously.
         *
         * @param path Path of the file to create.
         * @param access Requested access mode.
         * @return A valid file handle on success, otherwise nullptr.
         */
        std::shared_ptr<Handle> CreateFileSync(const std::string& path, Access access);

        /**
         * @brief Retrieves file information synchronously using a file path.
         *
         * @param path Path to the file.
         * @param info Output structure that receives the file information.
         * @return true if the information was retrieved successfully, otherwise false.
         */
        bool GetFileInfoSync(const std::string& path, FileInfo& info);

        /**
         * @brief Retrieves file information synchronously using an open file handle.
         *
         * @param handle Handle to the file.
         * @param info Output structure that receives the file information.
         * @return true if the information was retrieved successfully, otherwise false.
         */
        bool GetFileInfoSync(std::shared_ptr<Handle> handle, FileInfo& info);

        /**
         * @brief Removes a file synchronously.
         *
         * @param path Path to the file to remove.
         * @return true if the file was removed successfully, otherwise false.
         */
        bool RemoveFileSync(const std::string& path);

        /**
         * @brief Reads data from an open file synchronously.
         *
         * @param handle Handle to the file.
         * @param buffer Destination buffer that receives the read data.
         * @param[inout] count Maximum number of bytes to read and returns
         * actuall transfered bytes readed .
         * @param eof Set to true if the end of the file was reached.
         * @return true if the read was successfully, otherwise false..
         */
        bool ReadFileSync(
            std::shared_ptr<Handle> handle,
            void* buffer,
            size_t& count,
            bool& eof
        );

        /**
         * @brief Changes the current file position synchronously.
         *
         * @param[in] handle Handle to the file.
         * @param[inout] pos Offset used for the seek operation and returned with
         * the actuall pos value on the file .
         * @param mode Reference position used for seeking.
         * @return true if the seek was successfully, otherwise false..
         */
        bool SeekFileSync(
            std::shared_ptr<Handle> handle,
            size_t& pos,
            SeekMode mode
        );

        /**
         * @brief Retrieves the current file position synchronously.
         *
         * @param[in]  handle Handle to the file.
         * @param[out] pos The tell position . 
         * @return true if the tell was successfully, otherwise false.
         */
        bool TellFileSync(std::shared_ptr<Handle> handle , size_t& pos);

        /**
         * @brief Writes data to an open file synchronously.
         *
         * @param handle Handle to the file.
         * @param buffer Source buffer containing the data to write.
         * @param[inout] count Number of bytes to write. And Returns
         * actuall number of bytes writed .
         * @return true if the write was successfully, otherwise false..
         */
        bool WriteFileSync(
            std::shared_ptr<Handle> handle,
            const void* buffer,
            size_t& count
        );

        /**
         * @brief Closes an open file synchronously.
         *
         * @param handle Handle to the file.
         * @return true if the file was closed successfully, otherwise false.
         */
        bool CloseFileSync(std::shared_ptr<Handle> handle);

        /**
         * @brief Reads the contents of a directory synchronously.
         *
         * @param path Path to the directory.
         * @param entries Output container that receives the directory entries.
         * @return true if the directory was read successfully, otherwise false.
         */
        bool ReadDirSync(
            const std::string& path,
            std::vector<DirectoryEntry>& entries
        );

        /**
         * @brief Creates a directory synchronously.
         *
         * @param path Path of the directory to create.
         * @return true if the directory was created successfully, otherwise false.
         */
        bool CreateDirSync(const std::string& path);

        /**
         * @brief Removes a directory synchronously.
         *
         * @param path Path to the directory to remove.
         * @return true if the directory was removed successfully, otherwise false.
         */
        bool RemoveDirSync(const std::string& path);


















        /**
         * @brief Queues an asynchronous request to open an existing file.
         *
         * @param path Path to the file.
         * @param access Requested access mode.
         * @param callback Callback invoked when the operation completes.
         * @return true if the request was accepted and queued, otherwise false.
         */
        bool OpenFileAsync(
            const std::string& path,
            Access access,
            RequestCallBack callback
        );

        /**
         * @brief Queues an asynchronous request to create a file.
         *
         * @param path Path of the file to create.
         * @param access Requested access mode.
         * @param callback Callback invoked when the operation completes.
         * @return true if the request was accepted and queued, otherwise false.
         */
        bool CreateFileAsync(
            const std::string& path,
            Access access,
            RequestCallBack callback
        );

        /**
         * @brief Queues an asynchronous request to retrieve file information using a path.
         *
         * @param path Path to the file.
         * @param callback Callback invoked when the operation completes.
         * @return true if the request was accepted and queued, otherwise false.
         */
        bool GetFileInfoAsync(
            const std::string& path,
            RequestCallBack callback
        );

        /**
         * @brief Queues an asynchronous request to retrieve file information using an open handle.
         *
         * @param handle Handle to the file.
         * @param callback Callback invoked when the operation completes.
         * @return true if the request was accepted and queued, otherwise false.
         */
        bool GetFileInfoAsync(
            std::shared_ptr<Handle> handle,
            RequestCallBack callback
        );

        /**
         * @brief Queues an asynchronous request to remove a file.
         *
         * @param path Path to the file to remove.
         * @param callback Callback invoked when the operation completes.
         * @return true if the request was accepted and queued, otherwise false.
         */
        bool RemoveFileAsync(
            const std::string& path,
            RequestCallBack callback
        );

        /**
         * @brief Queues an asynchronous request to read data from a file.
         *
         * @param handle Handle to the file.
         * @param buffer Destination buffer that receives the read data.
         * @param count Maximum number of bytes to read.
         * @param callback Callback invoked when the operation completes.
         * @return true if the request was accepted and queued, otherwise false.
         *
         * @note The buffer must remain valid until the completion callback is invoked.
         */
        bool ReadFileAsync(
            std::shared_ptr<Handle> handle,
            void* buffer,
            size_t count,
            RequestCallBack callback
        );

        /**
         * @brief Queues an asynchronous request to change the current file position.
         *
         * @param handle Handle to the file.
         * @param pos Offset used for the seek operation.
         * @param mode Reference position used for seeking.
         * @param callback Callback invoked when the operation completes.
         * @return true if the request was accepted and queued, otherwise false.
         */
        bool SeekFileAsync(
            std::shared_ptr<Handle> handle,
            size_t pos,
            SeekMode mode,
            RequestCallBack callback
        );

        /**
         * @brief Queues an asynchronous request to retrieve the current file position.
         *
         * @param handle Handle to the file.
         * @param callback Callback invoked when the operation completes.
         * @return true if the request was accepted and queued, otherwise false.
         */
        bool TellFileAsync(
            std::shared_ptr<Handle> handle,
            RequestCallBack callback
        );

        /**
         * @brief Queues an asynchronous request to write data to a file.
         *
         * @param handle Handle to the file.
         * @param buffer Source buffer containing the data to write.
         * @param count Number of bytes to write.
         * @param callback Callback invoked when the operation completes.
         * @return true if the request was accepted and queued, otherwise false.
         *
         * @note The buffer must remain valid until the completion callback is invoked.
         */
        bool WriteFileAsync(
            std::shared_ptr<Handle> handle,
            const void* buffer,
            size_t count,
            RequestCallBack callback
        );

        /**
         * @brief Queues an asynchronous request to close an open file.
         *
         * @param handle Handle to the file.
         * @param callback Callback invoked when the operation completes.
         * @return true if the request was accepted and queued, otherwise false.
         */
        bool CloseFileAsync(
            std::shared_ptr<Handle> handle,
            RequestCallBack callback
        );

        /**
         * @brief Queues an asynchronous request to read the contents of a directory.
         *
         * @param path Path to the directory.
         * @param callback Callback invoked when the operation completes.
         * @return true if the request was accepted and queued, otherwise false.
         */
        bool ReadDirAsync(
            const std::string& path,
            RequestCallBack callback
        );

        /**
         * @brief Queues an asynchronous request to create a directory.
         *
         * @param path Path of the directory to create.
         * @param callback Callback invoked when the operation completes.
         * @return true if the request was accepted and queued, otherwise false.
         */
        bool CreateDirAsync(
            const std::string& path,
            RequestCallBack callback
        );

        /**
         * @brief Queues an asynchronous request to remove a directory.
         *
         * @param path Path to the directory to remove.
         * @param callback Callback invoked when the operation completes.
         * @return true if the request was accepted and queued, otherwise false.
         */
        bool RemoveDirAsync(
            const std::string& path,
            RequestCallBack callback
        );
	};

}
