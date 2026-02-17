#pragma once
#include <cstdarg>
#include <cstdint>
#include <cstdlib>

#if defined(_MSC_VER)
#include <intrin.h>
#define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__)
#define DEBUG_BREAK() __builtin_trap()
#else
#error "Unsupported compiler"
#endif

#ifdef _DEBUG
#define STOP_ON_ERROR() DEBUG_BREAK()
#else
#define STOP_ON_ERROR() std::abort()
#endif

namespace MultiStation {

	enum class LogLevel : uint8_t { Trace, Info, Warn, Error, Fatal };

	// init/shutdown του logger backend (spdlog μένει κρυφό στο .cpp)
	void LogInit();
	void LogShutdown();

	// printf-style formatting (C variadic) — δεν εκθέτει spdlog headers
	void EngineLogf(LogLevel lvl, const char* fmt, ...) noexcept;
	void ClientLogf(LogLevel lvl, const char* fmt, ...) noexcept;

} // namespace MultiStation

// Engine macros
#define MS_ENGINE_TRACE(...) ::MultiStation::EngineLogf(::MultiStation::LogLevel::Trace, __VA_ARGS__)
#define MS_ENGINE_INFO(...)  ::MultiStation::EngineLogf(::MultiStation::LogLevel::Info,  __VA_ARGS__)
#define MS_ENGINE_WARN(...)  ::MultiStation::EngineLogf(::MultiStation::LogLevel::Warn,  __VA_ARGS__)
#define MS_ENGINE_ERROR(...) ::MultiStation::EngineLogf(::MultiStation::LogLevel::Error, __VA_ARGS__)
#define MS_ENGINE_FATAL(...) ::MultiStation::EngineLogf(::MultiStation::LogLevel::Fatal, __VA_ARGS__)

// Client macros
#define MS_TRACE(...) ::MultiStation::ClientLogf(::MultiStation::LogLevel::Trace, __VA_ARGS__)
#define MS_INFO(...)  ::MultiStation::ClientLogf(::MultiStation::LogLevel::Info,  __VA_ARGS__)
#define MS_WARN(...)  ::MultiStation::ClientLogf(::MultiStation::LogLevel::Warn,  __VA_ARGS__)
#define MS_ERROR(...) ::MultiStation::ClientLogf(::MultiStation::LogLevel::Error, __VA_ARGS__)
#define MS_FATAL(...) ::MultiStation::ClientLogf(::MultiStation::LogLevel::Fatal, __VA_ARGS__)

#ifdef _DEBUG
#define MS_ASSERT(expr, ...) \
		if (!(expr)) { \
			MS_FATAL("Assertion failed (%s) at %s:%d. Message: %s" , #expr, __FILE__ , __LINE__ ,  __VA_ARGS__ ); \
			STOP_ON_ERROR(); \
		} 
#elif defined(NDEBUG)
	#define MS_ASSERT(expr, ...) if (!expr) { (STOP_ON_ERROR()) }
#endif