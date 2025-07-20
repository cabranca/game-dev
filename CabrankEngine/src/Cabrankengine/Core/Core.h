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

constexpr uint32_t bit(int x) { return 1u << x; }
