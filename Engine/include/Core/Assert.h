#pragma once

#include "Log.h"
#define ENGINE_ENABLE_ASSERTS
#ifdef ENGINE_ENABLE_ASSERTS
    #define ENGINE_ASSERT(x,...) {if(!(x)){ENGINE_ERROR("Assertion Failed:{0}",__VA_ARGS__);raise(SIGTRAP);}}
    #define ENGINE_CORE_ASSERT(x,...) {if(!(x)){ENGINE_CORE_ERROR("Assertion Failed:{0}",__VA_ARGS__);raise(SIGTRAP);}}
#else
    #define ENGINE_ASSERT(x,...)
    #define ENGINE_CORE_ASSERT(x,...)
#endif
// #ifndef ASSERT_H
// #define ASSERT_H

// // Ensure these macros are defined
// #define ENGINE_EXPAND_MACRO(x) x
// #define ENGINE_INTERNAL_ASSERT_WITH_MSG(condition, msg) if (!(condition)) { /* handle error */ }

// #endif // ASSERT_H