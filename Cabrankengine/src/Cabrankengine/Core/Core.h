#pragma once

#include <cstdint>

#ifdef CE_PLATFORM_WINDOWS
	#if CE_DYNAMIC_LINK
		#ifdef CE_BUILD_DLL
			#define CABRANKENGINE_API __declspec(dllexport)
		#else
			#define CABRANKENGINE_API __declspec(dllimport)
		#endif
	#else
		#define CABRANKENGINE_API
	#endif
#elif defined(CE_PLATFORM_LINUX)
	#if CE_DYNAMIC_LINK
		#ifdef CE_BUILD_DLL
			#define CABRANKENGINE_API __attribute__((visibility("default")))
		#else
			#define CABRANKENGINE_API
		#endif
	#else
		#define CABRANKENGINE_API
	#endif
#else
	#error Cabrankengine only supports Windows and Linux!
#endif

// Will need to update to support Linux since __debugbreak doesn't exist there
#ifdef CE_ENABLE_ASSERTS
	#define CE_ASSERT(x, ...) { if(!(x)) { CE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define CE_CORE_ASSERT(x, ...) { if(!(x)) { CE_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define CE_ASSERT(x, ...)
	#define CE_CORE_ASSERT(x, ...)
#endif

// Shifts a bit to the left. Replaced the macro with this to enforce type checking (and to practice)
// TODO: why 32 bits? Could it be different? (maybe due to sizeof(int))
constexpr uint32_t bit(int x) { return 1u << x; }

#define CE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1) // TODO: check a more modern alternative.
