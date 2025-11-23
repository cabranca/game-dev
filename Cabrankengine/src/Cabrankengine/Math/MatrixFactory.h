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
	Mat4 inverse(const Mat4& mat) noexcept;

	inline constexpr Mat4 zeroMat() noexcept {
		return {};
	}

	inline constexpr Mat4 identityMat() noexcept {
		return { 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f };
	}

	inline constexpr Mat4 scale(float uniform) noexcept {
		return { uniform, 0.f, 0.f, 0.f, 0.f, uniform, 0.f, 0.f, 0.f, 0.f, uniform, 0.f, 0.f, 0.f, 0.f, 1.f };
	}

	inline constexpr Mat4 scale(const Vector3& vec) noexcept {
		return { vec.x, 0.f, 0.f, 0.f, 0.f, vec.y, 0.f, 0.f, 0.f, 0.f, vec.z, 0.f, 0.f, 0.f, 0.f, 1.f };
	}

	inline constexpr Mat4 translate(const Vector3& vec) noexcept {
		return { 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, vec.x, vec.y, vec.z, 1.f };
	}

    inline Mat4 rotateX(float angle) noexcept {
		float angleInRadians = angle * PI / 180.f;
		return { 1.f, 0.f, 0.f, 0.f, 0.f, cosf(angleInRadians), sinf(angleInRadians), 0.f, 0.f, -sinf(angleInRadians), cosf(angleInRadians),
			     0.f, 0.f, 0.f, 0.f, 1.f
		};
	}

	inline Mat4 rotateY(float angle) noexcept {
		float angleInRadians = angle * PI / 180.f;
		return { cosf(angleInRadians), 0.f, -sinf(angleInRadians), 0.f, 0.f, 1.f, 0.f, 0.f,
			     sinf(angleInRadians), 0.f, cosf(angleInRadians), 0.f, 0.f, 0.f, 0.f, 1.f
		};
	}

	inline Mat4 rotateZ(float angle) noexcept {
		float angleInRadians = angle * PI / 180.f;
		return { cosf(angleInRadians), sinf(angleInRadians), 0.f,
			     0.f,
			     -sinf(angleInRadians),
			     cosf(angleInRadians),
			     0.f,
			     0.f,
			     0.f,
			     0.f,
			     1.f,
			     0.f,
			     0.f,
			     0.f,
			     0.f, 1.f
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
			2.f / rl, 0.f, 0.f, -(right + left) / rl, 
			0.f, 2.f / tb, 0.f, -(top + bottom) / tb, 
			0.f, 0.f, -2.f / fn, -(farZ + nearZ) / fn, 
			0.f, 0.f, 0.f, 1.f
		};
	}

	inline Mat4 perspective(float fovy, float aspect, float nearZ, float farZ) noexcept {
		float f = 1.0f / std::tan(fovy * 0.5f);
		float nf = nearZ - farZ;

		return Mat4 {
			f / aspect, 0.f, 0.f, 0.f,
			0.f,        f,   0.f, 0.f,
			0.f,        0.f, (farZ + nearZ) / nf, -1.f,
			0.f,        0.f, (2.f * farZ * nearZ) / nf, 0.f
		};
	}

    inline Mat4 inverse(const Mat4& m) noexcept {
		// Transpose rotation part.
		Mat4 inv{};
		inv.elements[0] = Vector3{ m.elements[0].x, m.elements[1].x, m.elements[2].x };
		inv.elements[1] = Vector3{ m.elements[0].y, m.elements[1].y, m.elements[2].y };
		inv.elements[2] = Vector3{ m.elements[0].z, m.elements[1].z, m.elements[2].z };

		// Invert translation
		const Vector4& t = m.elements[3];
		inv.elements[3].x = -(t.x * inv.elements[0].x + t.y * inv.elements[1].x + t.z * inv.elements[2].x);
		inv.elements[3].y = -(t.x * inv.elements[0].y + t.y * inv.elements[1].y + t.z * inv.elements[2].y);
		inv.elements[3].z = -(t.x * inv.elements[0].z + t.y * inv.elements[1].z + t.z * inv.elements[2].z);

		return inv;
	}

    inline std::array<float, 16> toGLMatrix(const Mat4& m) noexcept {
		const auto& e = m.elements;
		std::array<float, 16> res;
		for(int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				res[i*4+j] = e.at(i)[j];
			}
		}
		return res;
	}
} // namespace cabrankengine::math
