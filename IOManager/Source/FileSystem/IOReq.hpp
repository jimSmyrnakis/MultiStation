#pragma once
#include "../Globals.hpp"
/**
 * @author Dimitris smyrnakis
 * @brief Basic Functions Generalized the io operations using a Request / response scheme
 * , a way for fether abstraction in the future . 
 */
namespace MultiStation{

	// OPERATIONS FLAGS
#define EMPTY_IF_EXISTS (uint32_t)0x00000001

	namespace IO {

		ErrorCode ReadReq(Request& req, Response& res) noexcept;

		ErrorCode WriteReq(Request& req, Response& res) noexcept;

		ErrorCode FlushReq(Request& req, Response& res) noexcept;

		ErrorCode OpenReq(Request& req, Response& res) noexcept;

		ErrorCode CreateReq(Request& req, Response& res) noexcept;

		ErrorCode CloseReq(Request& req, Response& res) noexcept;

		ErrorCode SeekReq(Request& req, Response& res) noexcept;

		ErrorCode TellReq(Request& req, Response& res) noexcept;

		ErrorCode InfoReq(Request& req, Response& res) noexcept;

		ErrorCode MakeDirReq(Request& req, Response& res) noexcept;

		ErrorCode RmveDirReq(Request& req, Response& res) noexcept;

		ErrorCode ReadDirReq(Request& req, Response& res) noexcept;





		ErrorCode ExecuteOperation(Request& req, Response& res) noexcept;
	}

}
