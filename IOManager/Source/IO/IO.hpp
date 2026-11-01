#pragma once
#include "../Globals.hpp"
#include <string>
#include "../FileSystem/Handle/Handle.hpp"
/**
 * @author Dimitris Smyrnakis
 * @file IO.hpp
 * @brief All io operations for use in the engine are here . 
 * @version 0.1
 */
namespace MultiStation{

	namespace IO {

		/**
		 * @brief Creates a file on the defined filename path if not exist's
		 * @param filename The file path and name relative or absolute 
		 * @param empty_if_exists If true and file exist's already then deletes it's
		 * contents otherwise it remains unttached .
		 * @returns 
		 * SUCCESS if file created , 
		 * FILE_EXISTS if file already exist's or
		 * OPERATION_FAILED If file couldn't created .
		 */
		ErrorCode Create(const std::string& filename , bool empty_if_exists);

		ErrorCode Info(const std::string& filename, FileInfo& info);
		/**
		 * @brief Opens only an existing file with the required access mode 
		 * @param filename The file relative or absolute path
		 * @param access The access mode 
		 * @param[out] id Output id given (updated on success only)
		 * @returns 
		 * 1. SUCCESS : If file opened \n
		 * 2. FILE_NOT_EXISTS : If file not exists \n
		 * 3. OPERATION_FAILED: If file couldn't open for other reasons \n
		 * @note Access mode 
		 * 1. WRITE		: Destroyes containt and starts from zero pos .\n
		 * 2. READ		: Read file from zero pos\n
		 * 3. APPEND	: Write to file starting to the end of the file .\n
		 * 4. BINNARY	: For no handling on special new line characters (like 
		 * "\n" etc. that by default are handled .\n
		 * 5. APPEND overcomes WRITE (READ | WRITE | APPEND == READ | APPEND )\n
		 * 6. READ | WRITE : Read/Write the file at the current position , starting 
		 * from zero .\n
		 * 7. READ | APPEND: Read/Write the file but read starts from zero and write
		 * is always at eof position .\n
		 * 
		 */
		ErrorCode Open(const std::string& filename , Access access , int& id);
		/**
		 * @brief Reads bytes from the file in the buffer with the specified count 
		 * via the id that points to the file handle 
		 * @param id The id of the file handler 
		 * @param buffer The buffer address to write the file containts
		 * @param count Number of bytes to readed from the file
		 * @param[out] readed The actuall number of bytes readed from file
		 * @param[out] eof True if hit end of the file , false otherwise.
		 * @returns 
		 * 1. SUCCESS : If we succesfully read bytes from the file (readed <= count) or 
		 * found eof .\n
		 * 2. BAD_PARAMS : If buffer is null or count is zero \n
		 * 3. BAD_ID : If id is not a existing file handle\n
		 * 4. OPERATION_FAILED : If read failed for unknown reasons .\n
		 */
		ErrorCode Read(int id, void* buffer, uint32_t count, uint32_t& readed , bool& eof);

		ErrorCode Write(int id, void* buffer, uint32_t count, uint32_t& writed);

		ErrorCode Flush(int id);

		ErrorCode Seek(int id, int64_t pos, SeekMode mode, int64_t& final_pos);

		ErrorCode Tell(int id, int64_t& pos);
		/**
		 * @brief Closes the file given from the specified id of the file handle
		 * @param id Id to the implementation file handle
		 * @returns 
		 * 1. SUCCESS : If closed
		 * 2. OPERATION_FAILED: If closing wasn't valid 
		 */
		ErrorCode Close(int id);




		
		 


	}

}
