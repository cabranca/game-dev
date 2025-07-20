#pragma once

#include <cstdint>

#ifdef CE_PLATFORM_WINDOWS
	#ifdef CE_BUILD_DLL
		#define CABRANKENGINE_API __declspec(dllexport)
	#else
		#define CABRANKENGINE_API __declspec(dllimport)
	#endif
#else
	#error Cabrankengine only supports Windows!
#endif

#ifdef CE_ENABLE_ASSERTS
	#define CE_ASSERT(x, ...) { if(!(x)) { CE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define CE_CORE_ASSERT(x, ...) { if(!(x)) { CE_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define CE_ASSERT(x, ...)
	#define CE_CORE_ASSERT(x, ...)
#endif

constexpr uint32_t bit(int x) { return 1u << x; }
