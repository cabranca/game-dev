#include <catch.hpp>
#include <Cabrankengine/Math/Vector3.h>

using namespace cabrankengine::math;

// ====================================================
// Vector3fS
// ====================================================
TEST_CASE("Vector3 Arithmetic Operations") {
	Vector3 v{ 3, 4, 0 };

	SECTION("Binary Addition") {
		auto result = Vector3::Right + Vector3::Up;
		REQUIRE(result.x == 1.f);
		REQUIRE(result.y == 1.f);
		REQUIRE(result.z == 0.f);
	}

	SECTION("Unary Addition") {
		v += Vector3::Backward;
		REQUIRE(v.x == 3.f);
		REQUIRE(v.y == 4.f);
		REQUIRE(v.z == 1.f);
	}

	SECTION("Binary Subtraction") {
		auto result = Vector3::Right - Vector3::Up;
		REQUIRE(result.x == 1.f);
		REQUIRE(result.y == -1.f);
		REQUIRE(result.z == 0.f);
	}

	SECTION("Unary Subtraction") {
		v -= Vector3::Backward;
		REQUIRE(v.x == 3.f);
		REQUIRE(v.y == 4.f);
		REQUIRE(v.z == -1.f);
	}
}

TEST_CASE("Vector3 Scalar Multiplication") {
	Vector3 v{ 1, -2, 3 };

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

	SECTION("Multiplying by zero gives zero Vector3") {
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
		Vector3 a{ 1, 2, 3 };
		Vector3 b{ 4, 5, 6 };
		float s = 2.f;
		auto left = (a + b) * s;
		auto right = (a * s) + (b * s);
		REQUIRE(left.x == Approx(right.x).margin(EPSILON));
		REQUIRE(left.y == Approx(right.y).margin(EPSILON));
		REQUIRE(left.z == Approx(right.z).margin(EPSILON));
	}
}

TEST_CASE("Vector3 Unary Negation") {
	Vector3 v{ 1, -2, 3 };

	SECTION("Negation flips all signs") {
		auto neg = -v;
		REQUIRE(neg.x == Approx(-1.f).margin(EPSILON));
		REQUIRE(neg.y == Approx(2.f).margin(EPSILON));
		REQUIRE(neg.z == Approx(-3.f).margin(EPSILON));
	}

	SECTION("Double negation returns original Vector3") {
		auto neg = -(-v);
		REQUIRE(neg.x == Approx(v.x).margin(EPSILON));
		REQUIRE(neg.y == Approx(v.y).margin(EPSILON));
		REQUIRE(neg.z == Approx(v.z).margin(EPSILON));
	}

	SECTION("Negation preserves length") {
		auto neg = -v;
		REQUIRE(v.length() == Approx(neg.length()).margin(EPSILON));
	}

	SECTION("Negating zero Vector3 stays zero") {
		Vector3 zero{ 0, 0, 0 };
		auto neg = -zero;
		REQUIRE(neg.x == 0.f);
		REQUIRE(neg.y == 0.f);
		REQUIRE(neg.z == 0.f);
	}
}

TEST_CASE("Vector3 Length and Normalization") {
	SECTION("Length of standard axes") {
		REQUIRE(Vector3::Right.length() == Approx(1.f).margin(EPSILON));
		REQUIRE(Vector3::Up.length() == Approx(1.f).margin(EPSILON));
		REQUIRE(Vector3::Backward.length() == Approx(1.f).margin(EPSILON));
		REQUIRE(Vector3::Zero.length() == Approx(0.f).margin(EPSILON));
	}

	SECTION("Length of arbitrary Vector3") {
		Vector3 v{ 3, 4, 0 };
		REQUIRE(v.length() == Approx(5.f).margin(EPSILON));
	}

	SECTION("Normalize non-zero Vector3") {
		Vector3 v{ 3, 4, 0 };
		v.normalize();
		REQUIRE(v.length() == Approx(1.f).margin(EPSILON));
		REQUIRE(v.x == Approx(0.6f).margin(EPSILON));
		REQUIRE(v.y == Approx(0.8f).margin(EPSILON));
	}

	SECTION("Normalize zero Vector3 is no-op") {
		Vector3 v{ 0, 0, 0 };
		v.normalize();
		REQUIRE(v.x == 0.f);
		REQUIRE(v.y == 0.f);
		REQUIRE(v.z == 0.f);
	}

	SECTION("Double normalization keeps same value") {
		Vector3 v{ 1, 2, 3 };
		v.normalize();
		Vector3 before = v;
		v.normalize();
		REQUIRE(v.x == Approx(before.x).margin(EPSILON));
		REQUIRE(v.y == Approx(before.y).margin(EPSILON));
		REQUIRE(v.z == Approx(before.z).margin(EPSILON));
	}
}

TEST_CASE("Vector3 Dot Product") {
	SECTION("Orthogonal Vector3fs give 0") {
		REQUIRE(Vector3::Right.dot(Vector3::Up) == Approx(0.f).margin(EPSILON));
	}

	SECTION("Parallel Vector3fs give product of magnitudes") {
		Vector3 v1{ 2, 0, 0 };
		Vector3 v2{ 3, 0, 0 };
		REQUIRE(v1.dot(v2) == Approx(6.f).margin(EPSILON));
	}

	SECTION("Opposite Vector3fs give negative result") {
		Vector3 v1{ 1, 0, 0 };
		Vector3 v2{ -1, 0, 0 };
		REQUIRE(v1.dot(v2) == Approx(-1.f).margin(EPSILON));
	}
}

TEST_CASE("Vector3 Cross Product") {
	SECTION("Cross product of orthogonal basis Vector3fs") {
		auto result = Vector3::Right.cross(Vector3::Up);
		REQUIRE(result.x == Approx(0.f).margin(EPSILON));
		REQUIRE(result.y == Approx(0.f).margin(EPSILON));
		REQUIRE(result.z == Approx(1.f).margin(EPSILON));
	}

	SECTION("Cross product is anti-commutative") {
		REQUIRE(Vector3::Right.cross(Vector3::Up).z == Approx(1.f).margin(EPSILON));
		REQUIRE(Vector3::Up.cross(Vector3::Right).z == Approx(-1.f).margin(EPSILON));
	}

	SECTION("Cross product of parallel Vector3fs is zero") {
		auto result = Vector3::Right.cross(Vector3::Right);
		REQUIRE(result.length() == Approx(0.f).margin(EPSILON));
	}

	SECTION("Result is orthogonal to both operands") {
		Vector3 a{ 1, 2, 3 };
		Vector3 b{ 4, 5, 6 };
		Vector3 c = a.cross(b);
		REQUIRE(c.dot(a) == Approx(0.f).margin(EPSILON));
		REQUIRE(c.dot(b) == Approx(0.f).margin(EPSILON));
	}
}

TEST_CASE("Vector3 Normalization Check") {
	SECTION("Normalized Vector3 reports true") {
		Vector3 v{ 1, 0, 0 };
		REQUIRE(v.isNormalized());
	}

	SECTION("Non-normalized Vector3 reports false") {
		Vector3 v{ 2, 0, 0 };
		REQUIRE_FALSE(v.isNormalized());
	}

	SECTION("Normalized after normalize()") {
		Vector3 v{ 123456, 6547849, 0 };
		v.normalize();
		REQUIRE(v.isNormalized());
	}
}
