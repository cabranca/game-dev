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
	Vector3 v{ 3, 4, 0 };
	REQUIRE(v.lengthSquared() == Approx(25.f).margin(EPSILON));

	Vector3 zero = Vector3::Zero;
	REQUIRE(zero.lengthSquared() == Approx(0.f).margin(EPSILON));

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
	SECTION("Orthogonal vectors return true") {
		REQUIRE(perpendicular(Vector3::Right, Vector3::Up));
		REQUIRE(perpendicular(Vector3::Up, Vector3::Forward));
		REQUIRE(perpendicular(Vector3::Forward, Vector3::Right));
	}

	SECTION("Non-orthogonal vectors return false") {
		REQUIRE_FALSE(perpendicular(Vector3{ 1, 1, 0 }, Vector3::Up));
	}

	SECTION("Parallel Vector3fs give product of magnitudes") {
		REQUIRE(dot({ 2, 0, 0 }, { 3, 0, 0 }) == Approx(6.f).margin(EPSILON));
	}

	SECTION("Opposite Vector3fs give negative result") {
		REQUIRE(dot({ 1, 0, 0 }, { -1, 0, 0 }) == Approx(-1.f).margin(EPSILON));
	}
}

TEST_CASE("Vector3 Cross Product") {
	SECTION("Cross product of orthogonal basis Vector3fs") {
		auto result = cross(Vector3::Right, Vector3::Up);
		REQUIRE(result.x == Approx(0.f).margin(EPSILON));
		REQUIRE(result.y == Approx(0.f).margin(EPSILON));
		REQUIRE(result.z == Approx(1.f).margin(EPSILON));
	}

	SECTION("Cross product is anti-commutative") {
		REQUIRE(cross(Vector3::Right, Vector3::Up).z == Approx(1.f).margin(EPSILON));
		REQUIRE(cross(Vector3::Up, Vector3::Right).z == Approx(-1.f).margin(EPSILON));
	}

	SECTION("Cross product of parallel Vector3fs is zero") {
		auto result = cross(Vector3::Right, Vector3::Right);
		REQUIRE(result.length() == Approx(0.f).margin(EPSILON));
	}

	SECTION("Result is orthogonal to both operands") {
		Vector3 a{ 1, 2, 3 };
		Vector3 b{ 4, 5, 6 };
		Vector3 c = cross(a, b);
		REQUIRE(dot(c, a) == Approx(0.f).margin(EPSILON));
		REQUIRE(dot(c, b) == Approx(0.f).margin(EPSILON));
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

TEST_CASE("Vector3 Constants") {
	REQUIRE(Vector3::Right == Vector3{ 1, 0, 0 });
	REQUIRE(Vector3::Left == Vector3{ -1, 0, 0 });
	REQUIRE(Vector3::Up == Vector3{ 0, 1, 0 });
	REQUIRE(Vector3::Down == Vector3{ 0, -1, 0 });
	REQUIRE(Vector3::Forward == Vector3{ 0, 0, -1 });
	REQUIRE(Vector3::Backward == Vector3{ 0, 0, 1 });
	REQUIRE(Vector3::Zero == Vector3{ 0, 0, 0 });
	REQUIRE(Vector3::One == Vector3{ 1, 1, 1 });
}

TEST_CASE("Vector3 Equality and Inequality") {
	Vector3 a{ 1, 2, 3 };
	Vector3 b{ 1, 2, 3 };
	Vector3 c{ -1, 2, 3 };

	SECTION("Identical Vector3s are equal") {
		REQUIRE(a == b);
	}

	SECTION("Different Vector3s are not equal") {
		REQUIRE(a != c);
	}
}

TEST_CASE("Vector3 Angle Between") {
	SECTION("Parallel vectors have 0 radians between them") {
		REQUIRE(angleBetween(Vector3::Right, Vector3::Right) == Approx(0.f).margin(EPSILON));
	}

	SECTION("Opposite vectors have pi radians between them") {
		REQUIRE(angleBetween(Vector3::Right, Vector3::Left) == Approx(static_cast<float>(PI)).margin(EPSILON));
	}

	SECTION("Perpendicular vectors have pi/2 radians between them") {
		REQUIRE(angleBetween(Vector3::Right, Vector3::Up) == Approx(static_cast<float>(HALF_PI)).margin(EPSILON));
	}

	SECTION("Angle with zero vector returns 0") {
		REQUIRE(angleBetween(Vector3::Zero, Vector3::Right) == Approx(0.f).margin(EPSILON));
	}
}
