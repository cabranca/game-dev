#pragma once

#include <array>

#include "Constants.h"
#include "Vector4.h"

namespace cabrankengine::math {

    // This struct stores a 4x3 matrix representing a 4x4 matrix.
    // The last column is elided as it is trivial in computation.

	struct Mat4 {
        std::array<Vector4, 4> elements;

		constexpr Mat4() noexcept
            : elements {
                Vector4(),
                Vector4(),
                Vector4(),
                Vector4()
            } {}

		constexpr Mat4(float ix, float iy, float iz, float iw, float jx, float jy, float jz, float jw, float kx, float ky, float kz, float kw, float tx, float ty,
		               float tz, float tw) noexcept
		    : elements{
                Vector4(ix, iy, iz, iw),
                Vector4(jx, jy, jz, jw),
                Vector4(kx, ky, kz, kw),
                Vector4(tx, ty, tz, tw)
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

	inline constexpr Mat4 Mat4::transpose() const noexcept {
		auto& e = elements;
		return { e[0].x, e[1].x, e[2].x, e[3].x, e[0].y, e[1].y, e[2].y, e[3].y,
			     e[0].z, e[1].z, e[2].z, e[3].z, e[0].w, e[1].w, e[2].w, e[3].w };
	}

    // Row convention for the Vector
	inline constexpr Vector3 transformPoint(const Vector3& v, const Mat4& m) noexcept {
		auto trans = m.transpose();
		return Vector3{ dot(v, trans.elements[0]), dot(v, trans.elements[1]), dot(v, trans.elements[2])  } /*+ m.elements[3]*/;
	}

    // Row convention for the Vector
    inline constexpr Vector3 transformDirection(const Vector3& v, const Mat4& m) noexcept {
		auto trans = m.transpose();
		return Vector3{ dot(v, trans.elements[0]), dot(v, trans.elements[1]), dot(v, trans.elements[2]) } /*+ m.elements[3]*/;
	}

    inline constexpr Vector3 operator*(const Vector3& v, const Mat4& m) noexcept {
		return transformPoint(v, m);
	}
} // namespace cabrankengine::math
