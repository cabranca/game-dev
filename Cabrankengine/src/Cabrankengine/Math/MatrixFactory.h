#pragma once

#include "Mat4.h"

namespace cabrankengine::math {

	inline constexpr Mat4 zeroMat() {
		return {};
	}

	inline constexpr Mat4 identityMat() {
		return { 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f };
	}

	// Transpose only the upper 3x3 rotation part.
	// Translation is reset to zero.
	inline constexpr Mat4 Mat4::transpose() const noexcept {
		auto& e = elements;
		return { e[0].x, e[1].x, e[2].x, e[0].y, e[1].y, e[2].y, e[0].z, e[1].z, e[2].z, 0.f, 0.f, 0.f };
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

	inline Mat4 rotate(const Vector3& euler) noexcept {
		Mat4 rx = rotateX(euler.x);
		Mat4 ry = rotateY(euler.y);
		Mat4 rz = rotateZ(euler.z);

		return rz * ry * rx;
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

	inline Mat4 ortho(float left, float right, float bottom, float top, float nearZ, float farZ) noexcept {
		float rl = right - left;
		float tb = top - bottom;
		float fn = farZ - nearZ;

		return {
			2.f / rl, 0.f, 0.f, 0.f, 2.f / tb, 0.f, 0.f, 0.f, -2.f / fn, -(right + left) / rl, -(top + bottom) / tb, -(farZ + nearZ) / fn
		};
	}
} // namespace cabrankengine::math
