#pragma once

#include <array>

#include "Constants.h"
#include "Vector3.h"

namespace cabrankengine::math {

    // This struct stores a 4x3 matrix representing a 4x4 matrix.
    // The last column is elided as it is trivial in computation.

	struct Mat4 {
        std::array<Vector3, 4> elements;

		constexpr Mat4() noexcept
            : elements {
                Vector3(),
                Vector3(),
                Vector3(),
                Vector3()
            } {}

		constexpr Mat4(float ix, float iy, float iz, float jx, float jy, float jz, float kx, float ky, float kz, float tx, float ty,
		               float tz) noexcept
		    : elements{
                Vector3(ix, iy, iz),
                Vector3(jx, jy, jz),
                Vector3(kx, ky, kz),
                Vector3(tx, ty, tz)
            } {}

		constexpr bool operator==(const Mat4& other) const noexcept;
		constexpr bool operator!=(const Mat4& other) const noexcept;
		constexpr Mat4 operator*(const Mat4& other) const noexcept;
		constexpr Mat4 transpose() const noexcept;
	};

    constexpr bool Mat4::operator==(const Mat4& other) const noexcept {
		return elements == other.elements;
	}

	constexpr bool Mat4::operator!=(const Mat4& other) const noexcept {
		return elements != other.elements;
	}

	// Multiplies two affine 4x4 matrices stored as 4x3 (last column implicitly [0 0 0 1]).
	constexpr Mat4 Mat4::operator*(const Mat4& other) const noexcept {
		Mat4 transposed = other.transpose();
		Mat4 res{};
		// Rotational part
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j)
				res.elements[i][j] = dot(elements[i], transposed.elements[j]);
		}

		// Translation part (4th row)
		res.elements[3].x = dot(elements[3], transposed.elements[0]) + other.elements[3].x;
		res.elements[3].y = dot(elements[3], transposed.elements[1]) + other.elements[3].y;
		res.elements[3].z = dot(elements[3], transposed.elements[2]) + other.elements[3].z;
		return res;
	}

    // Transpose only the upper 3x3 rotation part.
	// Translation is reset to zero.
	inline constexpr Mat4 Mat4::transpose() const noexcept {
		auto& e = elements;
		return { e[0].x, e[1].x, e[2].x, e[0].y, e[1].y, e[2].y, e[0].z, e[1].z, e[2].z, 0.f, 0.f, 0.f };
	}
} // namespace cabrankengine::math
