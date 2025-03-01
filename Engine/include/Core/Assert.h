#pragma once

#include "Log.h"      // This header should define ENGINE_CORE_ERROR as usual.
#include <signal.h>

#ifdef _WIN32
    #define ENGINE_DEBUG_BREAK() __debugbreak()
#else
    #define ENGINE_DEBUG_BREAK() raise(SIGTRAP)
#endif

#define ENGINE_ENABLE_ASSERTS

#ifdef ENGINE_ENABLE_ASSERTS
#if __cplusplus >= 202002L
    // C++20 version: if __VA_ARGS__ is empty, __VA_OPT__ removes the preceding comma.
    // When no custom message is provided, the macro prints a default message using the condition.
    #define ENGINE_CORE_ASSERT(condition, ...)                             \
        do {                                                                 \
            if (!(condition)) {                                              \
                /* If no extra arguments are provided, use #condition as message */ \
                ENGINE_CORE_ERROR("Assertion Failed: " __VA_OPT__(__VA_ARGS__) __VA_OPT__(, ) "%s", #condition); \
                ENGINE_DEBUG_BREAK();                                        \
            }                                                                \
        } while(0)
#else
    // Fallback for pre-C++20 compilers: this version supports up to 10 arguments.
    // (You can extend the number if needed.)
    #define VA_SIZE_IMPL(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10, size, ...) size
    #define VA_SIZE(...) VA_SIZE_IMPL(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

    #define ENGINE_CORE_ASSERT_CHOOSER(...) ENGINE_CORE_ASSERT_CHOOSER_IMPL(__VA_ARGS__, \
         ENGINE_CORE_ASSERT_IMPL10, ENGINE_CORE_ASSERT_IMPL9, ENGINE_CORE_ASSERT_IMPL8, ENGINE_CORE_ASSERT_IMPL7, ENGINE_CORE_ASSERT_IMPL6, ENGINE_CORE_ASSERT_IMPL5, ENGINE_CORE_ASSERT_IMPL4, ENGINE_CORE_ASSERT_IMPL3, ENGINE_CORE_ASSERT_IMPL2, ENGINE_CORE_ASSERT_IMPL1)
    #define ENGINE_CORE_ASSERT_CHOOSER_IMPL(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,NAME,...) NAME

    #define ENGINE_CORE_ASSERT_IMPL1(condition)                                 \
        do {                                                                    \
            if (!(condition)) {                                                 \
                ENGINE_CORE_ERROR("Assertion Failed: %s", #condition);          \
                ENGINE_DEBUG_BREAK();                                           \
            }                                                                   \
        } while(0)
    #define ENGINE_CORE_ASSERT_IMPL2(condition, msg)                            \
        do {                                                                    \
            if (!(condition)) {                                                 \
                ENGINE_CORE_ERROR("Assertion Failed: " msg);                    \
                ENGINE_DEBUG_BREAK();                                           \
            }                                                                   \
        } while(0)
    #define ENGINE_CORE_ASSERT_IMPL3(condition, msg, a1)                        \
        do {                                                                    \
            if (!(condition)) {                                                 \
                ENGINE_CORE_ERROR("Assertion Failed: " msg, a1);                 \
                ENGINE_DEBUG_BREAK();                                           \
            }                                                                   \
        } while(0)
    #define ENGINE_CORE_ASSERT_IMPL4(condition, msg, a1, a2)                    \
        do {                                                                    \
            if (!(condition)) {                                                 \
                ENGINE_CORE_ERROR("Assertion Failed: " msg, a1, a2);             \
                ENGINE_DEBUG_BREAK();                                           \
            }                                                                   \
        } while(0)
    #define ENGINE_CORE_ASSERT_IMPL5(condition, msg, a1, a2, a3)                \
        do {                                                                    \
            if (!(condition)) {                                                 \
                ENGINE_CORE_ERROR("Assertion Failed: " msg, a1, a2, a3);         \
                ENGINE_DEBUG_BREAK();                                           \
            }                                                                   \
        } while(0)
    // (Define further implementations if you expect more than 5 extra arguments.)
    
    #define ENGINE_CORE_ASSERT(...)                                             \
        ENGINE_CORE_ASSERT_CHOOSER(__VA_ARGS__)(__VA_ARGS__)
#endif
#else
    #define ENGINE_CORE_ASSERT(...)
#endif
