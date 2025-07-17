#pragma once

#ifdef CE_PLATFORM_WINDOWS
	#ifdef CE_BUILD_DLL
		#define CABRANKEGINE_API __declspec(dllexport)
	#else
		#define CABRANKENGINE_API __declspec(dllimport)
	#endif
#else
	#error Cabrankengine only supports Windows!
#endif