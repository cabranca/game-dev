#pragma once

#include "Mat4.h"

namespace cabrankengine::math {

    constexpr Mat4 zeroMat() noexcept;
	constexpr Mat4 identityMat() noexcept;
    constexpr Mat4 scale(float uniform) noexcept;
	constexpr Mat4 scale(const Vector3&) noexcept;
	constexpr Mat4 translate(const Vector3&) noexcept;
	Mat4 rotateX(float angle) noexcept;
	Mat4 rotateY(float angle) noexcept;
	Mat4 rotateZ(float angle) noexcept;
	Mat4 rotate(const Vector3& euler) noexcept;
	Mat4 ortho(float left, float right, float bottom, float top, float nearZ, float farZ) noexcept;

	inline constexpr Mat4 zeroMat() noexcept {
		return {};
	}

	inline constexpr Mat4 identityMat() noexcept {
		return { 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f };
	}

	inline constexpr Mat4 scale(float uniform) noexcept {
		return { uniform, 0.f, 0.f, 0.f, uniform, 0.f, 0.f, 0.f, uniform, 0.f, 0.f, 0.f };
	}

	inline constexpr Mat4 scale(const Vector3& vec) noexcept {
		return { vec.x, 0.f, 0.f, 0.f, vec.y, 0.f, 0.f, 0.f, vec.z, 0.f, 0.f, 0.f };
	}

	inline constexpr Mat4 translate(const Vector3& vec) noexcept {
		return { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, vec.x, vec.y, vec.z };
	}

    inline Mat4 rotateX(float angle) noexcept {
		float angleInRadians = angle * PI / 180.f;
		return {
			1.f, 0.f, 0.f, 0.f, cos(angleInRadians), sin(angleInRadians), 0.f, -sin(angleInRadians), cos(angleInRadians), 0.f, 0.f, 0.f
		};
	}

	inline Mat4 rotateY(float angle) noexcept {
		float angleInRadians = angle * PI / 180.f;
		return {
			cos(angleInRadians), 0.f, -sin(angleInRadians), 0.f, 1.f, 0.f, sin(angleInRadians), 0.f, cos(angleInRadians), 0.f, 0.f, 0.f
		};
	}

	inline Mat4 rotateZ(float angle) noexcept {
		float angleInRadians = angle * PI / 180.f;
		return {
			cos(angleInRadians), sin(angleInRadians), 0.f, -sin(angleInRadians), cos(angleInRadians), 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f
		};
	}

	inline Mat4 rotate(const Vector3& euler) noexcept {
		Mat4 rx = rotateX(euler.x);
		Mat4 ry = rotateY(euler.y);
		Mat4 rz = rotateZ(euler.z);

		return rz * ry * rx;
	}

	inline Mat4 ortho(float left, float right, float bottom, float top, float nearZ, float farZ) noexcept {
		float rl = right - left;
		float tb = top - bottom;
		float fn = farZ - nearZ;

		return {
			2.f / rl, 0.f, 0.f, 0.f, 2.f / tb, 0.f, 0.f, 0.f, -2.f / fn, -(right + left) / rl, -(top + bottom) / tb, -(farZ + nearZ) / fn
		};
	}
} // namespace cabrankengine::math
