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

#define CE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1) // TODO: check a more modern alternative.

namespace cabrankengine {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}
