#pragma once

#include "Vector3.h"

namespace cabrankengine::math {

    struct Vector4 {
		float x, y, z, w;

		constexpr Vector4() noexcept : x(), y(), z(), w() {}
		explicit constexpr Vector4(float uniform) noexcept : x(uniform), y(uniform), z(uniform), w(uniform) {}
		constexpr Vector4(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}
		constexpr Vector4(const Vector3& v, float w = 0.f) noexcept : x(v.x), y(v.y), z(v.z), w(w) {}

		constexpr float lengthSquared() const noexcept;
		float length() const noexcept;
		void normalize() noexcept;
		bool isNormalized() const noexcept;

		constexpr bool operator==(const Vector4& other) const noexcept;
		constexpr bool operator!=(const Vector4& other) const noexcept;
		constexpr Vector4 operator+(const Vector4& other) const noexcept;
		constexpr Vector4& operator+=(const Vector4& other) noexcept;
		constexpr Vector4 operator-(const Vector4& other) const noexcept;
		constexpr Vector4& operator-=(const Vector4& other) noexcept;
		constexpr Vector4 operator-() const noexcept;
		constexpr Vector4 operator*(float scale) const noexcept;
		constexpr Vector4& operator*=(float scale) noexcept;
		constexpr float& operator[](int index) noexcept;
		constexpr const float& operator[](int index) const noexcept;

		constexpr Vector3 toVector3() const noexcept;
	};

    inline constexpr float Vector4::lengthSquared() const noexcept {
		return x * x + y * y + z * z + w * w;
	}

	inline float Vector4::length() const noexcept {
		return std::sqrt(lengthSquared());
	}

	inline void Vector4::normalize() noexcept {
		float squared = lengthSquared();
		if (squared == 0.f)
			return;
		float len = std::sqrt(squared);
		x /= len;
		y /= len;
		z /= len;
		w /= len;
	}

    inline bool Vector4::isNormalized() const noexcept {
		float result = std::abs(lengthSquared() - 1.f);
		return result < EPSILON;
	}

	inline constexpr bool Vector4::operator==(const Vector4& other) const noexcept {
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	inline constexpr bool Vector4::operator!=(const Vector4& other) const noexcept {
		return !(*this == other);
	}

	inline constexpr Vector4 Vector4::operator+(const Vector4& other) const noexcept {
		return { x + other.x, y + other.y, z + other.z, w + other.w };
	}

	inline constexpr Vector4& Vector4::operator+=(const Vector4& other) noexcept {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	inline constexpr Vector4 Vector4::operator-(const Vector4& other) const noexcept {
		return { x - other.x, y - other.y, z - other.z, w - other.w };
	}

	inline constexpr Vector4& Vector4::operator-=(const Vector4& other) noexcept {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	inline constexpr Vector4 Vector4::operator-() const noexcept {
		return { -x, -y, -z, -w };
	}

	inline constexpr Vector4 Vector4::operator*(float scale) const noexcept {
		return { x * scale, y * scale, z * scale, w * scale };
	}

	inline constexpr Vector4& Vector4::operator*=(float scale) noexcept {
		x *= scale;
		y *= scale;
		z *= scale;
		w *= scale;
		return *this;
	}

	inline constexpr float& Vector4::operator[](int index) noexcept {
		switch (index) {
		    case 0:
			    return x;
		    case 1:
			    return y;
		    case 2:
			    return z;
		    case 3:
			    return w;
		    default:
			    CE_CORE_ERROR("Trying to access Vector4 with invalid index!");
			    return x;
		}
	}

	inline constexpr const float& Vector4::operator[](int index) const noexcept {
		switch (index) {
		    case 0:
			    return x;
		    case 1:
			    return y;
		    case 2:
			    return z;
		    case 3:
			    return w;
		    default:
			    CE_CORE_ERROR("Trying to access Vector4 with invalid index!");
			    return x;
		}
	}

	inline constexpr Vector3 Vector4::toVector3() const noexcept {
		return { x, y, z };
	}

    inline constexpr float dot(const Vector4& a, const Vector4& b) noexcept {
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	inline constexpr bool perpendicular(const Vector4& a, const Vector4& b) noexcept {
		return dot(a, b) == 0;
	}

	inline float angleBetween(const Vector4& a, const Vector4& b) noexcept {
		float denom = a.length() * b.length();
		if (denom == 0.f)
			return 0.f;
		float cosTheta = dot(a, b) / denom;
		cosTheta = std::clamp(cosTheta, -1.f, 1.f);
		return std::acos(cosTheta);
	}
};
