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

    // Μορφοποίηση printf -> string, και μετά log σαν απλό μήνυμα (χωρίς fmt templates)
    void VLog(std::shared_ptr<spdlog::logger>& lg,
        spdlog::level::level_enum lvl,
        const char* fmt,
        va_list args) noexcept
    {
        if (!lg || !fmt) return;

        char stackbuf[1024];

        va_list args_copy;
        va_copy(args_copy, args);
        int needed = std::vsnprintf(stackbuf, sizeof(stackbuf), fmt, args_copy);
        va_end(args_copy);

        if (needed < 0) return;

        if (needed < (int)sizeof(stackbuf)) {
            lg->log(lvl, spdlog::string_view_t(stackbuf, (size_t)needed));
            return;
        }

        // Μεγάλο μήνυμα: allocate
        std::string heap;
        heap.resize((size_t)needed + 1);

        std::vsnprintf(heap.data(), heap.size(), fmt, args);
        lg->log(lvl, spdlog::string_view_t(heap.data(), (size_t)needed));
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

    void EngineLogf(LogLevel lvl, const char* fmt, ...) noexcept {
        va_list args;
        va_start(args, fmt);
        VLog(g_engine, ToSpd(lvl), fmt, args);
        va_end(args);

       /* if (lvl == LogLevel::Fatal) {
#if defined(_DEBUG)
            DEBUG_BREAK();
#else 
            std::abort();
#endif
            
        }*/
    }

    void ClientLogf(LogLevel lvl, const char* fmt, ...) noexcept {
        va_list args;
        va_start(args, fmt);
        VLog(g_client, ToSpd(lvl), fmt, args);
        va_end(args);

        
    }

} // namespace MultiStation
