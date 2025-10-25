#pragma once

#include <cmath>

namespace cabrankengine::math {

	constexpr float EPSILON = 1e-5f;

	struct Vector {
		float x, y, z;

		constexpr Vector() : x(), y(), z() {}

		constexpr Vector(float uniform) : x(uniform), y(uniform), z(uniform) {}

		constexpr Vector(float x, float y, float z) : x(x), y(y), z(z) {}

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

		constexpr Vector operator+(const Vector& other) const noexcept {
			return { x + other.x, y + other.y, z + other.z };
		}

		constexpr Vector& operator+=(const Vector& other) noexcept {
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		constexpr Vector operator-(const Vector& other) const noexcept {
			return { x - other.x, y - other.y, z - other.z };
		}

		constexpr Vector& operator-=(const Vector& other) noexcept {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		constexpr Vector operator-() {
			return { -x, -y, -z };
		}

		constexpr Vector operator*(float scale) const noexcept {
			return { x * scale, y * scale, z * scale };
		}

		constexpr Vector& operator*=(float scale) {
			x *= scale;
			y *= scale;
			z *= scale;
			return *this;
		}

		constexpr float dot(const Vector& other) const noexcept {
			return x * other.x + y * other.y + z * other.z;
		}

		constexpr Vector cross(const Vector& other) const noexcept {
			Vector res{};
			res.x = y * other.z - z * other.y;
			res.y = z * other.x - x * other.z;
			res.z = x * other.y - y * other.x;
			return res;
		}
	};

} // namespace cabrankengine::math
