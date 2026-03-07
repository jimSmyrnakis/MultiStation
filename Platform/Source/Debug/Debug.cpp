#include "Debug.hpp"

#include <memory>
#include <string>
#include <cstdio>
#include <cstdarg>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace {
    std::shared_ptr<spdlog::logger> g_engine;
    std::shared_ptr<spdlog::logger> g_client;

    spdlog::level::level_enum ToSpd(MultiStation::LogLevel lvl) {
        switch (lvl) {
        case MultiStation::LogLevel::Trace: return spdlog::level::trace;
        case MultiStation::LogLevel::Info:  return spdlog::level::info;
        case MultiStation::LogLevel::Warn:  return spdlog::level::warn;
        case MultiStation::LogLevel::Error: return spdlog::level::err;
        case MultiStation::LogLevel::Fatal: return spdlog::level::critical;
        default: return spdlog::level::info;
        }
    }

    void VLog(std::shared_ptr<spdlog::logger>& lg,
        spdlog::level::level_enum lvl,
        int line,
        const char* file,
        const char* fmt,
        va_list args) noexcept
    {
        if (!lg || !fmt) return;

        char msgbuf[1024];

        va_list args_copy;
        va_copy(args_copy, args);
        int needed = std::vsnprintf(msgbuf, sizeof(msgbuf), fmt, args_copy);
        va_end(args_copy);

        if (needed < 0) return;

        std::string message;

        if (needed < (int)sizeof(msgbuf)) {
            message.assign(msgbuf, (size_t)needed);
        }
        else {
            std::string heap;
            heap.resize((size_t)needed + 1);

            std::vsnprintf(heap.data(), heap.size(), fmt, args);
            message.assign(heap.data(), (size_t)needed);
        }

        // τελικό μήνυμα με file / line
        char finalbuf[1280];
        int final_len = std::snprintf(
            finalbuf,
            sizeof(finalbuf),
            "%s at line %d, file %s : %s",
            spdlog::level::to_string_view(lvl).data(),
            line,
            file ? file : "unknown",
            message.c_str()
        );

        if (final_len < 0) return;

        lg->log(lvl, spdlog::string_view_t(finalbuf, (size_t)std::min(final_len, (int)sizeof(finalbuf))));
    }
}

namespace MultiStation {

    void LogInit() {
        spdlog::set_pattern("%^%l [%n] : \"%v\" %c %$");

        g_engine = spdlog::stdout_color_mt("MS Engine");
        g_engine->set_level(spdlog::level::trace);

        g_client = spdlog::stdout_color_mt("Application");
        g_client->set_level(spdlog::level::trace);
    }

    void LogShutdown() {
        g_engine.reset();
        g_client.reset();
        spdlog::shutdown();
    }

    void EngineLogf(LogLevel lvl, int line, const char* file, const char* fmt, ...) noexcept {
        va_list args;
        va_start(args, fmt);
        VLog(g_engine, ToSpd(lvl),line , file , fmt, args);
        va_end(args);

       /* if (lvl == LogLevel::Fatal) {
#if defined(_DEBUG)
            DEBUG_BREAK();
#else 
            std::abort();
#endif
            
        }*/
    }

    void ClientLogf(LogLevel lvl, int line, const char* file, const char* fmt, ...) noexcept {
        va_list args;
        va_start(args, fmt);
        VLog(g_client, ToSpd(lvl),line , file , fmt, args);
        va_end(args);

        
    }

} // namespace MultiStation
