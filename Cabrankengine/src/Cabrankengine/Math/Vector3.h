#pragma once

#include <cmath>

namespace cabrankengine::math {

	constexpr float EPSILON = 1e-5f;

    template<typename T>
	struct Vector3 {
		T x, y, z;

		constexpr Vector3() : x(), y(), z() {}

		constexpr Vector3(T uniform) : x(uniform), y(uniform), z(uniform) {}

		constexpr Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

		float length() const noexcept {
			return sqrt(x * x + y * y + z * z);
		}

		void normalize() noexcept {
			float lengthSquared = x * x + y * y + z * z;
			if (lengthSquared == 0)
				return;
			float length = sqrt(lengthSquared);
			x /= length;
			y /= length;
			z /= length;
		}

		bool isNormalized() const noexcept {
			float result = abs(x * x + y * y + z * z - 1.f);
			return result < EPSILON;
		}

		constexpr Vector3 operator+(const Vector3& other) const noexcept {
			return { x + other.x, y + other.y, z + other.z };
		}

		constexpr Vector3& operator+=(const Vector3& other) noexcept {
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		constexpr Vector3 operator-(const Vector3& other) const noexcept {
			return { x - other.x, y - other.y, z - other.z };
		}

		constexpr Vector3& operator-=(const Vector3& other) noexcept {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		constexpr Vector3 operator-() {
			return { -x, -y, -z };
		}

		constexpr Vector3 operator*(float scale) const noexcept {
			return { x * scale, y * scale, z * scale };
		}

		constexpr Vector3& operator*=(float scale) {
			x *= scale;
			y *= scale;
			z *= scale;
			return *this;
		}

		constexpr float dot(const Vector3& other) const noexcept {
			return x * other.x + y * other.y + z * other.z;
		}

		constexpr Vector3 cross(const Vector3& other) const noexcept {
			Vector3 res{};
			res.x = y * other.z - z * other.y;
			res.y = z * other.x - x * other.z;
			res.z = x * other.y - y * other.x;
			return res;
		}
	};

    using Vector3f = Vector3<float>;
    using Vector3i = Vector3<int>;

} // namespace cabrankengine::math
