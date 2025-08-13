#pragma once

#include <cstdint>
#include <memory>

#ifdef _WIN32
	#ifdef _WIN64
		#define CE_PLATFORM_WINDOWS
	#else
		#error "Cabrankengine only supports 64-bit Windows!"
	#endif
#endif

#ifdef __linux__
	#ifdef __x86_64__
		#define CE_PLATFORM_LINUX
	#else
		#error "Cabrankengine only supports 64-bit Linux!"
	#endif
#endif

#ifdef CE_DEBUG
	#define CE_ENABLE_ASSERTS
	#if defined(CE_PLATFORM_WINDOWS)
		#define CE_DEBUG_BREAK __debugbreak()
	#elif defined(CE_PLATFORM_LINUX)
		#include <signal.h>
		#define CE_DEBUG_BREAK raise(SIGTRAP)
	#endif
#endif


// Will need to update to support Linux since __debugbreak doesn't exist there
#ifdef CE_ENABLE_ASSERTS
	#define CE_ASSERT(x, ...) { if(!(x)) { CE_ERROR("Assertion failed: {0}", __VA_ARGS__); CE_DEBUG_BREAK; } }
	#define CE_CORE_ASSERT(x, ...) { if(!(x)) { CE_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); CE_DEBUG_BREAK; } }
#else
	#define CE_ASSERT(x, ...)
	#define CE_CORE_ASSERT(x, ...)
#endif


// Shifts a bit to the left. Replaced the macro with this to enforce type checking (and to practice)
// The return type is uint32_t to ensure it can be used in bitwise operations. It allows 32 disjoint flags.
constexpr uint32_t BIT(int x) { return 1u << x; }


// Binds a function to an event. This is used to pass member functions as callbacks.
// It uses perfect forwarding to preserve the value category of the function and the event.
template<typename T, typename U>
auto BIND_EVENT_FN(T&& func, U* instance) {
	return [func = std::forward<T>(func), instance](auto&& event) -> decltype(auto) {
		return (instance->*func)(std::forward<decltype(event)>(event));
		};
}


// This usings are useless now but when we want to make our own smart pointers,
// we can just change them here and they will be used everywhere.
namespace cabrankengine {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename... Args>
	Scope<T> createScope(Args&&... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename... Args>
	Ref<T> createRef(Args&&... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
