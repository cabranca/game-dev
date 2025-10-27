#pragma once
#include <numbers>

namespace cabrankengine::math {
	inline constexpr float PI = std::numbers::pi_v<float>;
	inline constexpr float HALF_PI = std::numbers::pi_v<float> / 2.0f;
	inline constexpr float TWO_PI = std::numbers::pi_v<float> * 2.0f;
	inline constexpr float EPSILON = 1e-5f;
} // namespace cabrankengine::math
