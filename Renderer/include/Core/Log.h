#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Engine{
    class Log
    {
    private:
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ClientLogger;
    public:
        static void Init();

        inline static Ref<spdlog::logger>& GetCoreLoggger(){return s_CoreLogger;}
        inline static Ref<spdlog::logger>& GetClientLoggger(){return s_ClientLogger;}
    };
}

#define ENGINE_CORE_TRACE(...) ::Engine::Log::GetCoreLoggger()->trace(__VA_ARGS__)
#define ENGINE_CORE_INFO(...) ::Engine::Log::GetCoreLoggger()->info(__VA_ARGS__)
#define ENGINE_CORE_WARN(...) ::Engine::Log::GetCoreLoggger()->warn(__VA_ARGS__)
#define ENGINE_CORE_ERROR(...) ::Engine::Log::GetCoreLoggger()->error(__VA_ARGS__)

//#define ENGINE_DEBUG_PRINT
#ifdef ENGINE_DEBUG_PRINT
#define ENGINE_DEBUG_TRACE(...) ENGINE_CORE_TRACE(__VA_ARGS__)
#else
#define ENGINE_DEBUG_TRACE(...)
#endif

#define ENGINE_TRACE(...) ::Engine::Log::GetClientLoggger()->trace(__VA_ARGS__)
#define ENGINE_INFO(...) ::Engine::Log::GetClientLoggger()->info(__VA_ARGS__)
#define ENGINE_WARN(...) ::Engine::Log::GetClientLoggger()->warn(__VA_ARGS__)
#define ENGINE_ERROR(...) ::Engine::Log::GetClientLoggger()->error(__VA_ARGS__)