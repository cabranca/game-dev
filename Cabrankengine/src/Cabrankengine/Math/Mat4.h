#pragma once

#include <array>

#include "Constants.h"
#include "Vector3.h"

namespace cabrankengine::math {

    // This struct stores a 4x3 matrix representing a 4x4 matrix.
    // The last column is elided as it is trivial in computation.

	struct Mat4 {
        std::array<Vector3, 4> elements;

		static const Mat4 Zero;
		static const Mat4 Identity;
		static const float Rows;
		static const float Columns;

		static constexpr Mat4 scale(float uniform) noexcept;
		static constexpr Mat4 scale(const Vector3& vec) noexcept;
		static constexpr Mat4 rotate(const Vector3& vec) noexcept;
        static constexpr Mat4 translate(const Vector3& vec) noexcept;
		constexpr Mat4 transpose() const noexcept;

		constexpr Mat4() noexcept
            : elements{
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
	};

	inline constexpr Mat4 Mat4::Zero{};
	inline constexpr Mat4 Mat4::Identity {
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f,
        0.f, 0.f, 0.f
    };
	inline constexpr float Mat4::Rows{ 4 };
	inline constexpr float Mat4::Columns{ 3 };

    inline constexpr Mat4 Mat4::scale(float uniform) noexcept {
		return {
            uniform,   0.f,     0.f,
              0.f,   uniform,   0.f,
              0.f,     0.f,   uniform,
              0.f,     0.f,     0.f
        };
	}

    inline constexpr Mat4 Mat4::scale(const Vector3& vec) noexcept {
		return {
            vec.x,  0.f,   0.f,
             0.f,  vec.y,  0.f,
             0.f,   0.f,  vec.z,
             0.f,   0.f,   0.f
        };
	}

    inline constexpr Mat4 Mat4::rotate(const Vector3& vec) noexcept {
		return {};
    }

    inline constexpr Mat4 rotateX(float angle) noexcept {
		float angleInRadians = angle * PI / 180.f;
		return {
            1.f, 0.f, 0.f,
            0.f, cos(angleInRadians), sin(angleInRadians),
            0.f, -sin(angleInRadians), cos(angleInRadians),
            0.f, 0.f, 0.f
        };
	}

    inline constexpr Mat4 rotateY(float angle) noexcept {
		float angleInRadians = angle * PI / 180.f;
		return {
            cos(angleInRadians), 0.f, -sin(angleInRadians),
			0.f, 1.f, 0.f,
			sin(angleInRadians), 0.f, cos(angleInRadians),
            0.f, 0.f, 0.f
		};
	}

    inline constexpr Mat4 rotateZ(float angle) noexcept {
		float angleInRadians = angle * PI / 180.f;
		return {
            cos(angleInRadians), sin(angleInRadians), 0.f,
			-sin(angleInRadians), cos(angleInRadians), 0.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 0.f
		};
	}

	inline constexpr Mat4 Mat4::translate(const Vector3& vec) noexcept {
		return {
            0.f, 0.f, 0.f,
            0.f, 0.f, 0.f,
            0.f, 0.f, 0.f,
            vec.x, vec.y, vec.z
        };
	}

    inline constexpr Mat4 Mat4::transpose() const noexcept {
		auto& e = elements;
		return {
            e[0].x, e[1].x, e[2].x,
            e[0].y, e[1].y, e[2].y,
            e[0].z, e[1].z, e[2].z,
            0.f, 0.f, 0.f
        };
    }

	constexpr bool Mat4::operator==(const Mat4& other) const noexcept {
		return elements == other.elements;
	}

	constexpr bool Mat4::operator!=(const Mat4& other) const noexcept {
		return elements != other.elements;
	}

    constexpr Mat4 Mat4::operator*(const Mat4& other) const noexcept {
		Mat4 transposed = other.transpose();
		Mat4 res{};
		for (int i = 0; i < Rows; i++) {
            for (int j = 0; j < Columns; j++) {
				res.elements[i][j] = dot(elements[i], transposed.elements[j]);
            }
        }
		return res;
    }

} // namespace cabrankengine::math
