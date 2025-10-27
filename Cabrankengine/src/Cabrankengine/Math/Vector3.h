#pragma once

#include <cmath>

namespace cabrankengine::math {

	constexpr float EPSILON = 1e-5f;

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

		constexpr float dot(const Vector3& other) const noexcept;
		constexpr Vector3 cross(const Vector3& other) const noexcept;
		constexpr bool perpendicular(const Vector3& other) const noexcept;
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

	constexpr float Vector3::dot(const Vector3& other) const noexcept {
		return x * other.x + y * other.y + z * other.z;
	}

	constexpr Vector3 Vector3::cross(const Vector3& other) const noexcept {
		Vector3 res{};
		res.x = y * other.z - z * other.y;
		res.y = z * other.x - x * other.z;
		res.z = x * other.y - y * other.x;
		return res;
	}

	constexpr bool Vector3::perpendicular(const Vector3& other) const noexcept {
		return dot(other) == 0;
	}

} // namespace cabrankengine::math
