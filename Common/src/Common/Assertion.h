#pragma once

#include <Common/Logger.h>

#ifdef CE_DEBUG
	#define CE_ENABLE_ASSERTS
	#if defined(_WIN32) && defined(_WIN64)
		#define CE_DEBUG_BREAK __debugbreak()
	#elif defined(__linux__) || defined(__APPLE__)
		#include <signal.h>
		#define CE_DEBUG_BREAK raise(SIGTRAP)
	#endif
#endif

#ifdef CE_ENABLE_ASSERTS
	#define CE_ASSERT(x, ...)      { if(!(x)) { CE_ERROR("Assertion failed: {0}", __VA_ARGS__); CE_DEBUG_BREAK; } }
	#define CE_CORE_ASSERT(x, ...) { if(!(x)) { CE_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); CE_DEBUG_BREAK; } }
	#define CBK_AC_ASSERT(x, ...)  { if(!(x)) { CBK_AC_ERROR("Assertion failed: {0}", __VA_ARGS__); CE_DEBUG_BREAK; } }
#else
	#define CE_ASSERT(x, ...)
	#define CE_CORE_ASSERT(x, ...)
	#define CBK_AC_ASSERT(x, ...)
#endif
