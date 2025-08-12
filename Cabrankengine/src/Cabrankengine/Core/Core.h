#pragma once

#include <cstdint>
#include <memory>

#ifdef CE_DEBUG
	#define CE_ENABLE_ASSERTS
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
// The return type is uint32_t to ensure it can be used in bitwise operations. It allows 32 disjoint flags.
constexpr uint32_t bit(int x) { return 1u << x; }


// Binds a function to an event. This is used to pass member functions as callbacks.
// It uses perfect forwarding to preserve the value category of the function and the event.
template<typename T, typename U>
auto bind_event_fn(T&& func, U* instance) {
	return [func = std::forward<T>(func), instance](auto&& event) -> decltype(auto) {
		return (instance->*func)(std::forward<decltype(event)>(event));
		};
}


// This usings are useless now but when we want to make our own smart pointers,
// we can just change them here and they will be used everywhere.
namespace cabrankengine {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}
