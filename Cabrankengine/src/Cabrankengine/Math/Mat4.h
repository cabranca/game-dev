#pragma once

#include "Constants.h"
#include "Vector3.h"

namespace cabrankengine::math {

    // This struct stores a 4x3 matrix representing a 4x4 matrix.
    // The last column is elided as it is trivial in computation.

	struct Mat4 {
		float ix, iy, iz; // 0 //
		float jx, jy, jz; // 0 //
		float kx, ky, kz; // 0 //
		float tx, ty, tz; // 1 // The t vector represents the translation from the origin

		static const Mat4 Zero;
		static const Mat4 Identity;

		constexpr Mat4 translate(const Vector3& vec) noexcept;
		constexpr Mat4 scale(float uniform) noexcept;
		constexpr Mat4 scale(const Vector3& vec) noexcept;

		constexpr Mat4() noexcept : ix(), iy(), iz(), jx(), jy(), jz(), kx(), ky(), kz(), tx(), ty(), tz() {}
		constexpr Mat4(float ix, float iy, float iz, float jx, float jy, float jz, float kx, float ky, float kz, float tx, float ty,
		               float tz) noexcept
		    : ix(), iy(), iz(), jx(), jy(), jz(), kx(), ky(), kz(), tx(), ty(), tz() {}

		constexpr bool operator==(const Mat4& other) const noexcept;
		constexpr bool operator!=(const Mat4& other) const noexcept;
		constexpr Mat4 operator*(float scale) const noexcept;
		constexpr Mat4& operator*=(float scale) noexcept;
	};

	inline constexpr Mat4 Mat4::Zero{};
	inline constexpr Mat4 Mat4::Identity {
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f,
        0.f, 0.f, 0.f
    };

    inline constexpr Mat4 Mat4::translate(const Vector3& vec) {
		return {
             0.f,   0.f,   0.f,
             0.f,   0.f,   0.f,
             0.f,   0.f,   0.f,
            vec.x, vec.y, vec.z
        };
    }

    inline constexpr Mat4 Mat4::scale(float uniform) {
		return {
            uniform,   0.f,     0.f,
              0.f,   uniform,   0.f,
              0.f,     0.f,   uniform,
              0.f,     0.f,     0.f
        };
	}

    inline constexpr Mat4 Mat4::scale(const Vector3& vec) {
		return {
            vec.x,  0.f,   0.f,
             0.f,  vec.y,  0.f,
             0.f,   0.f,  vec.z,
             0.f,   0.f,   0.f
        };
	}

	constexpr bool Mat4::operator==(const Mat4& other) const noexcept {
		return ix == other.ix && iy == other.iy && iz == other.iz && jx == other.jx && jy == other.jy && jz == other.jz && kx == other.kx &&
		       ky == other.ky && kz == other.kz && tx == other.tx && ty == other.ty && tz == other.tz;
	}

	constexpr bool Mat4::operator!=(const Mat4& other) const noexcept {
		return ix == other.ix && iy == other.iy && iz == other.iz && jx == other.jx && jy == other.jy && jz == other.jz && kx == other.kx &&
		       ky == other.ky && kz == other.kz && tx == other.tx && ty == other.ty && tz == other.tz;
	}

} // namespace cabrankengine::math
