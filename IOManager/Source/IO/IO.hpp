#pragma once
#include <string>
#include "../Globals.hpp"
#include "IOManager.hpp"
namespace MultiStation{

	namespace IO {

		bool Init(void);

		void Fini(void);

		void PollCallbacks(void);





		Handle* OpenFileSync(const std::string& path , Access access);

		Handle* CreateFileSync(const std::string& path, Access access);

		bool	GetFileInfoSync(const std::string& path, FileInfo& info);

		bool	GetFileInfoSync(Handle* handle, FileInfo& info);

		bool    RemoveFileSync(const std::string& path);

		size_t  ReadFileSync(Handle* handle, void* buffer, size_t count , bool& eof);

		size_t  SeekFileSync(Handle* handle, size_t pos, SeekMode mode);

		size_t  TellFileSync(Handle* handle);

		size_t  WriteFileSync(Handle* handle, const void* buffer, size_t count);

		bool    CloseFileSync(Handle* handle);

		bool	ReadDirSync(const std::string& path, std::vector<DirectoryEntry>& entries);

		bool	CreateDirSync(const std::string& path);

		bool	RemoveDirSync(const std::string& path);




		bool OpenFileAsync(const std::string& path, Access access, RequestCallBack callback);

		bool CreateFileAsync(const std::string& path, Access access, RequestCallBack callback);

		bool GetFileInfoAsync(const std::string& path, RequestCallBack callback);

		bool GetFileInfoAsync(Handle* handle, RequestCallBack callback);

		bool RemoveFileAsync(const std::string& path, RequestCallBack callback);

		bool ReadFileAsync(Handle* handle, void* buffer, size_t count, RequestCallBack callback);

		bool SeekFileAsync(Handle* handle, size_t pos, SeekMode mode, RequestCallBack callback);

		bool TellFileAsync(Handle* handle, RequestCallBack callback);

		bool WriteFileAsync(Handle* handle, const void* buffer, size_t count, RequestCallBack callback);

		bool CloseFileAsync(Handle* handle, RequestCallBack callback);

		bool ReadDirAsync(const std::string& path, RequestCallBack callback);

		bool CreateDirAsync(const std::string& path, RequestCallBack callback);

		bool RemoveDirAsync(const std::string& path, RequestCallBack callback);

		extern IOManager s_manager;
	};

}
