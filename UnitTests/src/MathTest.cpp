#include <catch.hpp>
#include <Cabrankengine/Math/Vector.h>

using namespace cabrankengine::math;

// ----------------------------------------------------
// Helpers
// ----------------------------------------------------
constexpr Vector X_AXIS{ 1, 0, 0 };
constexpr Vector Y_AXIS{ 0, 1, 0 };
constexpr Vector Z_AXIS{ 0, 0, 1 };
constexpr Vector ZERO{ 0 };

// ====================================================
// VECTORS
// ====================================================
TEST_CASE("Vector Arithmetic Operations") {
	Vector v{ 3, 4, 0 };

	SECTION("Binary Addition") {
		auto result = X_AXIS + Y_AXIS;
		REQUIRE(result.x == 1.f);
		REQUIRE(result.y == 1.f);
		REQUIRE(result.z == 0.f);
	}

	SECTION("Unary Addition") {
		v += Z_AXIS;
		REQUIRE(v.x == 3.f);
		REQUIRE(v.y == 4.f);
		REQUIRE(v.z == 1.f);
	}

	SECTION("Binary Subtraction") {
		auto result = X_AXIS - Y_AXIS;
		REQUIRE(result.x == 1.f);
		REQUIRE(result.y == -1.f);
		REQUIRE(result.z == 0.f);
	}

	SECTION("Unary Subtraction") {
		v -= Z_AXIS;
		REQUIRE(v.x == 3.f);
		REQUIRE(v.y == 4.f);
		REQUIRE(v.z == -1.f);
	}
}

TEST_CASE("Vector Scalar Multiplication") {
	Vector v{ 1, -2, 3 };

	SECTION("Binary multiplication scales all components") {
		auto result = v * 2.f;
		REQUIRE(result.x == Approx(2.f).margin(EPSILON));
		REQUIRE(result.y == Approx(-4.f).margin(EPSILON));
		REQUIRE(result.z == Approx(6.f).margin(EPSILON));
	}

	SECTION("Unary multiplication assignment updates in place") {
		v *= 0.5f;
		REQUIRE(v.x == Approx(0.5f).margin(EPSILON));
		REQUIRE(v.y == Approx(-1.f).margin(EPSILON));
		REQUIRE(v.z == Approx(1.5f).margin(EPSILON));
	}

	SECTION("Multiplying by zero gives zero vector") {
		auto result = v * 0.f;
		REQUIRE(result.x == Approx(0.f).margin(EPSILON));
		REQUIRE(result.y == Approx(0.f).margin(EPSILON));
		REQUIRE(result.z == Approx(0.f).margin(EPSILON));
	}

	SECTION("Multiplying by -1 inverts direction") {
		auto result = v * -1.f;
		REQUIRE(result.x == Approx(-v.x).margin(EPSILON));
		REQUIRE(result.y == Approx(-v.y).margin(EPSILON));
		REQUIRE(result.z == Approx(-v.z).margin(EPSILON));
	}

	SECTION("Scalar multiplication is distributive") {
		Vector a{ 1, 2, 3 };
		Vector b{ 4, 5, 6 };
		float s = 2.f;
		auto left = (a + b) * s;
		auto right = (a * s) + (b * s);
		REQUIRE(left.x == Approx(right.x).margin(EPSILON));
		REQUIRE(left.y == Approx(right.y).margin(EPSILON));
		REQUIRE(left.z == Approx(right.z).margin(EPSILON));
	}
}

TEST_CASE("Vector Unary Negation") {
	Vector v{ 1, -2, 3 };

	SECTION("Negation flips all signs") {
		auto neg = -v;
		REQUIRE(neg.x == Approx(-1.f).margin(EPSILON));
		REQUIRE(neg.y == Approx(2.f).margin(EPSILON));
		REQUIRE(neg.z == Approx(-3.f).margin(EPSILON));
	}

	SECTION("Double negation returns original vector") {
		auto neg = -(-v);
		REQUIRE(neg.x == Approx(v.x).margin(EPSILON));
		REQUIRE(neg.y == Approx(v.y).margin(EPSILON));
		REQUIRE(neg.z == Approx(v.z).margin(EPSILON));
	}

	SECTION("Negation preserves length") {
		auto neg = -v;
		REQUIRE(v.length() == Approx(neg.length()).margin(EPSILON));
	}

	SECTION("Negating zero vector stays zero") {
		Vector zero{ 0, 0, 0 };
		auto neg = -zero;
		REQUIRE(neg.x == 0.f);
		REQUIRE(neg.y == 0.f);
		REQUIRE(neg.z == 0.f);
	}
}

TEST_CASE("Vector Length and Normalization") {
	SECTION("Length of standard axes") {
		REQUIRE(X_AXIS.length() == Approx(1.f).margin(EPSILON));
		REQUIRE(Y_AXIS.length() == Approx(1.f).margin(EPSILON));
		REQUIRE(Z_AXIS.length() == Approx(1.f).margin(EPSILON));
		REQUIRE(ZERO.length() == Approx(0.f).margin(EPSILON));
	}

	SECTION("Length of arbitrary vector") {
		Vector v{ 3, 4, 0 };
		REQUIRE(v.length() == Approx(5.f).margin(EPSILON));
	}

	SECTION("Normalize non-zero vector") {
		Vector v{ 3, 4, 0 };
		v.normalize();
		REQUIRE(v.length() == Approx(1.f).margin(EPSILON));
		REQUIRE(v.x == Approx(0.6f).margin(EPSILON));
		REQUIRE(v.y == Approx(0.8f).margin(EPSILON));
	}

	SECTION("Normalize zero vector is no-op") {
		Vector v{ 0, 0, 0 };
		v.normalize();
		REQUIRE(v.x == 0.f);
		REQUIRE(v.y == 0.f);
		REQUIRE(v.z == 0.f);
	}

	SECTION("Double normalization keeps same value") {
		Vector v{ 1, 2, 3 };
		v.normalize();
		Vector before = v;
		v.normalize();
		REQUIRE(v.x == Approx(before.x).margin(EPSILON));
		REQUIRE(v.y == Approx(before.y).margin(EPSILON));
		REQUIRE(v.z == Approx(before.z).margin(EPSILON));
	}
}

TEST_CASE("Vector Dot Product") {
	SECTION("Orthogonal vectors give 0") {
		REQUIRE(X_AXIS.dot(Y_AXIS) == Approx(0.f).margin(EPSILON));
	}

	SECTION("Parallel vectors give product of magnitudes") {
		Vector v1{ 2, 0, 0 };
		Vector v2{ 3, 0, 0 };
		REQUIRE(v1.dot(v2) == Approx(6.f).margin(EPSILON));
	}

	SECTION("Opposite vectors give negative result") {
		Vector v1{ 1, 0, 0 };
		Vector v2{ -1, 0, 0 };
		REQUIRE(v1.dot(v2) == Approx(-1.f).margin(EPSILON));
	}
}

TEST_CASE("Vector Cross Product") {
	SECTION("Cross product of orthogonal basis vectors") {
		auto result = X_AXIS.cross(Y_AXIS);
		REQUIRE(result.x == Approx(0.f).margin(EPSILON));
		REQUIRE(result.y == Approx(0.f).margin(EPSILON));
		REQUIRE(result.z == Approx(1.f).margin(EPSILON));
	}

	SECTION("Cross product is anti-commutative") {
		REQUIRE(X_AXIS.cross(Y_AXIS).z == Approx(1.f).margin(EPSILON));
		REQUIRE(Y_AXIS.cross(X_AXIS).z == Approx(-1.f).margin(EPSILON));
	}

	SECTION("Cross product of parallel vectors is zero") {
		auto result = X_AXIS.cross(X_AXIS);
		REQUIRE(result.length() == Approx(0.f).margin(EPSILON));
	}

	SECTION("Result is orthogonal to both operands") {
		Vector a{ 1, 2, 3 };
		Vector b{ 4, 5, 6 };
		Vector c = a.cross(b);
		REQUIRE(c.dot(a) == Approx(0.f).margin(EPSILON));
		REQUIRE(c.dot(b) == Approx(0.f).margin(EPSILON));
	}
}

TEST_CASE("Vector Normalization Check") {
	SECTION("Normalized vector reports true") {
		Vector v{ 1, 0, 0 };
		REQUIRE(v.isNormalized());
	}

	SECTION("Non-normalized vector reports false") {
		Vector v{ 2, 0, 0 };
		REQUIRE_FALSE(v.isNormalized());
	}

	SECTION("Normalized after normalize()") {
		Vector v{ 123456, 6547849, 0 };
		v.normalize();
		REQUIRE(v.isNormalized());
	}
}
