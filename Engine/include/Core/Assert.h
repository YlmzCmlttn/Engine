#pragma once

#include "Log.h"

// Detect the operating system and define a platform-specific debug break
#ifdef _WIN32
    // Windows: use the compiler intrinsic to trigger a breakpoint
#define ENGINE_DEBUG_BREAK() __debugbreak()
#else
    // Other OS (e.g., Linux, macOS): use raise with SIGTRAP
#include <signal.h>
#define ENGINE_DEBUG_BREAK() raise(SIGTRAP)
#endif

#define ENGINE_ENABLE_ASSERTS

#ifdef ENGINE_ENABLE_ASSERTS
#define ENGINE_ASSERT(x, ...) { if (!(x)) { ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); ENGINE_DEBUG_BREAK(); } }
#define ENGINE_CORE_ASSERT(x, ...) { if (!(x)) { ENGINE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); ENGINE_DEBUG_BREAK(); } }
#else
#define ENGINE_ASSERT(x, ...)
#define ENGINE_CORE_ASSERT(x, ...)
#endif

// #ifndef ASSERT_H
// #define ASSERT_H

// // Ensure these macros are defined
// #define ENGINE_EXPAND_MACRO(x) x
// #define ENGINE_INTERNAL_ASSERT_WITH_MSG(condition, msg) if (!(condition)) { /* handle error */ }

// #endif // ASSERT_H