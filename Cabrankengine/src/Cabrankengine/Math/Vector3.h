#pragma once

#include <cmath>
#include <algorithm>

#include <Cabrankengine/Core/Logger.h>

#include "Constants.h"

namespace cabrankengine::math {

	struct Vector3 {
		float x, y, z;

		static const Vector3 Right;
		static const Vector3 Left;
		static const Vector3 Up;
		static const Vector3 Down;
		static const Vector3 Forward;
		static const Vector3 Backward;
		static const Vector3 Zero;
		static const Vector3 One;

		constexpr Vector3() noexcept : x(), y(), z() {}
		explicit constexpr Vector3(float uniform) noexcept : x(uniform), y(uniform), z(uniform) {}
		constexpr Vector3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}

		constexpr float lengthSquared() const noexcept;
		float length() const noexcept;
		void normalize() noexcept;
		bool isNormalized() const noexcept;

		constexpr bool operator==(const Vector3& other) const noexcept;
		constexpr bool operator!=(const Vector3& other) const noexcept;
		constexpr Vector3 operator+(const Vector3& other) const noexcept;
		constexpr Vector3& operator+=(const Vector3& other) noexcept;
		constexpr Vector3 operator-(const Vector3& other) const noexcept;
		constexpr Vector3& operator-=(const Vector3& other) noexcept;
		constexpr Vector3 operator-() noexcept;
		constexpr Vector3 operator*(float scale) const noexcept;
		constexpr Vector3& operator*=(float scale) noexcept;
		constexpr float& operator[](int index) noexcept;
	};

    inline constexpr Vector3 Vector3::Right     { (1.f), (0.f), (0.f) };
	inline constexpr Vector3 Vector3::Left      { (-1.f), (0.f), (0.f) };
	inline constexpr Vector3 Vector3::Up        { (0.f), (1.f), (0.f) };
	inline constexpr Vector3 Vector3::Down      { (0.f), (-1.f), (0.f) };
	inline constexpr Vector3 Vector3::Forward   { (0.f), (0.f), (-1.f) };
	inline constexpr Vector3 Vector3::Backward  { (0.f), (0.f), (1.f) };
	inline constexpr Vector3 Vector3::Zero      { (0.f) };
	inline constexpr Vector3 Vector3::One       { (1.f) };

    constexpr float Vector3::lengthSquared() const noexcept {
		return x * x + y * y + z * z;
	}

	float Vector3::length() const noexcept {
		return sqrt(lengthSquared());
	}

	void Vector3::normalize() noexcept {
		float squared = lengthSquared();
		if (squared == 0)
			return;
		float length = sqrt(squared);
		x /= length;
		y /= length;
		z /= length;
	}

	bool Vector3::isNormalized() const noexcept {
		float result = abs(lengthSquared() - 1.f);
		return result < EPSILON;
	}

	constexpr bool Vector3::operator==(const Vector3& other) const noexcept {
		return x == other.x && y == other.y && z == other.z;
	}

	constexpr bool Vector3::operator!=(const Vector3& other) const noexcept {
		return x != other.x || y != other.y || z != other.z;
	}

	constexpr Vector3 Vector3::operator+(const Vector3& other) const noexcept {
		return { x + other.x, y + other.y, z + other.z };
	}

	constexpr Vector3& Vector3::operator+=(const Vector3& other) noexcept {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	constexpr Vector3 Vector3::operator-(const Vector3& other) const noexcept {
		return { x - other.x, y - other.y, z - other.z };
	}

	constexpr Vector3& Vector3::operator-=(const Vector3& other) noexcept {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	constexpr Vector3 Vector3::operator-() noexcept {
		return { -x, -y, -z };
	}

	constexpr Vector3 Vector3::operator*(float scale) const noexcept {
		return { x * scale, y * scale, z * scale };
	}

	constexpr Vector3& Vector3::operator*=(float scale) noexcept {
		x *= scale;
		y *= scale;
		z *= scale;
		return *this;
	}

	constexpr float& Vector3::operator[](int index) noexcept {
        switch (index) {
		    case 0:
			    return x;
		    case 1:
			    return y;
		    case 2:
			    return z;
		    default:
			    CE_CORE_ERROR("Trying to acces a Vector with invalid index!");
			    return x; // This must be handled in other way
        }
	}

	constexpr float dot(const Vector3& a, const Vector3& b) noexcept {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	constexpr Vector3 cross(const Vector3& a, const Vector3& b) noexcept {
		Vector3 res{};
		res.x = a.y * b.z - a.z * b.y;
		res.y = a.z * b.x - a.x * b.z;
		res.z = a.x * b.y - a.y * b.x;
		return res;
	}

	constexpr bool perpendicular(const Vector3& a, const Vector3& b) noexcept {
		return dot(a, b) == 0;
	}

    float angleBetween(const Vector3& a, const Vector3& b) noexcept {
		float denom = a.length() * b.length();
		if (denom == 0.f)
			return 0.f;
		float cosTheta = dot(a, b) / denom;
		cosTheta = std::clamp(cosTheta, -1.f, 1.f);
		return std::acos(cosTheta);
	}

} // namespace cabrankengine::math
