#pragma once

#include "Log.h"      // This header should define ENGINE_CORE_ERROR as usual.
#include <signal.h>

#define ENGINE_ENABLE_ASSERTS

#ifdef _WIN32
    #define ENGINE_DEBUG_BREAK() __debugbreak()
#else
    #define ENGINE_DEBUG_BREAK() raise(SIGTRAP)
#endif
#define ENGINE_EXPAND_VARGS(x) x

#ifdef ENGINE_ENABLE_ASSERTS
#define ENGINE_ASSERT_NO_MESSAGE(condition) { if(!(condition)) { ENGINE_ERROR("Assertion Failed"); ENGINE_DEBUG_BREAK(); } }
#define ENGINE_ASSERT_MESSAGE(condition, ...) { if(!(condition)) { ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); ENGINE_DEBUG_BREAK(); } }

#define ENGINE_ASSERT_RESOLVE(arg1, arg2, macro, ...) macro
#define ENGINE_GET_ASSERT_MACRO(...) ENGINE_EXPAND_VARGS(ENGINE_ASSERT_RESOLVE(__VA_ARGS__, ENGINE_ASSERT_MESSAGE, ENGINE_ASSERT_NO_MESSAGE))

#define ENGINE_ASSERT(...) ENGINE_EXPAND_VARGS( ENGINE_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
#define ENGINE_CORE_ASSERT(...) ENGINE_EXPAND_VARGS( ENGINE_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
#else
#define ENGINE_ASSERT(...)
#define ENGINE_CORE_ASSERT(...)
#endif

