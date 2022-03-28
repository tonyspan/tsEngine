#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "Engine/Core/Base.h"

namespace tsEngine
{
    class Log
    {
    public:
        static void Init();

        inline static Ref<spdlog::logger>& GetLogger() { return s_Logger; }
    private:
        Log(const Log& other) = delete;
        Log& operator=(Log& other) = delete;
    private:
        static Ref<spdlog::logger> s_Logger;
    };
}

#define LOG_INFO(...) ::tsEngine::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::tsEngine::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::tsEngine::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::tsEngine::Log::GetLogger()->critical(__VA_ARGS__)

#define ASSERT(x, ...) if(!(x)) { ::tsEngine::Log::GetLogger()->critical("{0}: {1}", __PRETTY_FUNCTION__, fmt::format(__VA_ARGS__)); DEBUG_BREAK; }
