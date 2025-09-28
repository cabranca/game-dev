#pragma once

#include <bitset>
#include <cstdint>

namespace {

	using Entity = uint32_t;

	constexpr Entity MAX_ENTITIES = 5000;
	constexpr size_t MAX_COMPONENTS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;
}

