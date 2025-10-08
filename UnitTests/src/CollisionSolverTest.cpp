#include <catch.hpp>
#include <glm/glm.hpp>

#include <Cabrankengine/Core/CollisionSolver.hpp>

using namespace cabrankengine;
using namespace::components;
using namespace cabrankengine::collision_solver;

// ----------------------------------------------------
// Helpers
// ----------------------------------------------------
template<typename VecType>
CCollisionBox<VecType> MakeBox(const VecType& size) {
    return CCollisionBox<VecType>(size);
}

// ====================================================
// AABB vs AABB
// ====================================================
TEST_CASE("AABB vs AABB - 2D Contact (no overlapping)") {
    glm::vec2 pos1(0, 0);
    auto b1 = MakeBox(glm::vec2(1, 1));

    glm::vec2 pos2(1, 0);
    auto b2 = MakeBox(glm::vec2(1, 1));

    auto result = AABB(pos1, b1, pos2, b2);

    REQUIRE(result.collided);
    REQUIRE(result.penetration == 0.0f);
}

TEST_CASE("AABB vs AABB - 2D Overlap") {
    glm::vec2 pos1(0, 0);
    auto b1 = MakeBox(glm::vec2(1, 1));

    glm::vec2 pos2(1, 0);
    auto b2 = MakeBox(glm::vec2(2, 2));

    auto result = AABB(pos1, b1, pos2, b2);

    REQUIRE(result.collided);
    REQUIRE(result.penetration > 0.0f);
}

TEST_CASE("AABB vs AABB - 2D No Collision") {
    glm::vec2 pos1(0, 0);
    auto b1 = MakeBox(glm::vec2(1, 1));

    glm::vec2 pos2(5, 0);
    auto b2 = MakeBox(glm::vec2(1, 1));

    auto result = AABB(pos1, b1, pos2, b2);

    REQUIRE_FALSE(result.collided);
}

TEST_CASE("AABB vs AABB - 3D Contact (no overlapping)") {
    glm::vec3 pos1(0, 0, 0);
    auto b1 = MakeBox(glm::vec3(1, 1, 1));

    glm::vec3 pos2(1, 1, 1);
    auto b2 = MakeBox(glm::vec3(1, 1, 1));

    auto result = AABB(pos1, b1, pos2, b2);

    REQUIRE(result.collided);
    REQUIRE(result.penetration == 0.0f);
}

TEST_CASE("AABB vs AABB - 3D Overlap") {
    glm::vec3 pos1(0, 0, 0);
    auto b1 = MakeBox(glm::vec3(1, 1, 1));

    glm::vec3 pos2(0.5f, 0.5f, 0.5f);
    auto b2 = MakeBox(glm::vec3(1, 1, 1));

    auto result = AABB(pos1, b1, pos2, b2);

    REQUIRE(result.collided);
    REQUIRE(result.penetration > 0.0f);
}

TEST_CASE("AABB vs AABB - 3D No Collision") {
    glm::vec3 pos1(0, 0, 0);
    auto b1 = MakeBox(glm::vec3(1, 1, 1));

    glm::vec3 pos2(5, 5, 5);
    auto b2 = MakeBox(glm::vec3(1, 1, 1));

    auto result = AABB(pos1, b1, pos2, b2);

    REQUIRE_FALSE(result.collided);
}

// ====================================================
// Sphere vs Sphere
// ====================================================
TEST_CASE("Sphere vs Sphere - 2D Contact (no overlapping)") {
    glm::vec2 pos1(0, 0);
    glm::vec2 pos2(2, 0);

    auto result = Sphere(pos1, 1.0f, pos2, 1.0f);

    REQUIRE(result.collided);
    REQUIRE(result.penetration == Approx(0.0f).margin(1e-5));
}

TEST_CASE("Sphere vs Sphere - 2D Overlap") {
    glm::vec2 pos1(0, 0);
    glm::vec2 pos2(1, 0);

    auto result = Sphere(pos1, 1.0f, pos2, 1.0f);

    REQUIRE(result.collided);
    REQUIRE(result.penetration == Approx(1.0f).margin(1e-5));
}

TEST_CASE("Sphere vs Sphere - 2D No Collision") {
    glm::vec2 pos1(0, 0);
    glm::vec2 pos2(5, 0);

    auto result = Sphere(pos1, 1.0f, pos2, 1.0f);

    REQUIRE_FALSE(result.collided);
}

TEST_CASE("Sphere vs Sphere - 3D Contact (no overlapping)") {
    glm::vec3 pos1(0, 0, 0);
    glm::vec3 pos2(2, 0, 0);

    auto result = Sphere(pos1, 1.0f, pos2, 1.0f);

    REQUIRE(result.collided);
    REQUIRE(result.penetration == Approx(0.0f).margin(1e-5));
}

TEST_CASE("Sphere vs Sphere - 3D Overlap") {
    glm::vec3 pos1(0, 0, 0);
    glm::vec3 pos2(1, 0, 0);

    auto result = Sphere(pos1, 1.0f, pos2, 1.0f);

    REQUIRE(result.collided);
    REQUIRE(result.penetration == Approx(1.0f).margin(1e-5));
}

TEST_CASE("Sphere vs Sphere - 3D No Collision") {
    glm::vec3 pos1(0, 0, 0);
    glm::vec3 pos2(5, 0, 0);

    auto result = Sphere(pos1, 1.0f, pos2, 1.0f);

    REQUIRE_FALSE(result.collided);
}

// ====================================================
// Sphere vs AABB
// ====================================================
TEST_CASE("Sphere vs AABB - 2D Contact (no overlapping)") {
    glm::vec2 circlePos(1, 0);
    float radius = 0.5f;

    glm::vec2 boxPos(0, 0);
    auto box = MakeBox(glm::vec2(1, 1));

    auto result = SphereAABB(circlePos, radius, boxPos, box);

    REQUIRE(result.collided);
    REQUIRE(result.penetration == 0.0f);
}

TEST_CASE("Sphere vs AABB - 2D Overlap") {
    glm::vec2 circlePos(0.5f, 0.5f);
    float radius = 1.0f;

    glm::vec2 boxPos(0, 0);
    auto box = MakeBox(glm::vec2(1, 1));

    auto result = SphereAABB(circlePos, radius, boxPos, box);

    REQUIRE(result.collided);
    REQUIRE(result.penetration > 0.0f);
}

TEST_CASE("Sphere vs AABB - 2D No Collision") {
    glm::vec2 circlePos(5, 5);
    float radius = 1.0f;

    glm::vec2 boxPos(0, 0);
    auto box = MakeBox(glm::vec2(1, 1));

    auto result = SphereAABB(circlePos, radius, boxPos, box);

    REQUIRE_FALSE(result.collided);
}

TEST_CASE("Sphere vs AABB - 3D Contact (no overlapping)") {
    glm::vec3 spherePos(1, 0, 0);
    float radius = 0.5f;

    glm::vec3 boxPos(0, 0, 0);
    auto box = MakeBox(glm::vec3(1, 1, 1));

    auto result = SphereAABB(spherePos, radius, boxPos, box);

    REQUIRE(result.collided);
    REQUIRE(result.penetration == 0.0f);
}

TEST_CASE("Sphere vs AABB - 3D Overlap") {
    glm::vec3 spherePos(1, 1, 1);
    float radius = 1.0f;

    glm::vec3 boxPos(0, 0, 0);
    auto box = MakeBox(glm::vec3(1, 1, 1));

    auto result = SphereAABB(spherePos, radius, boxPos, box);

    REQUIRE(result.collided);
    REQUIRE(result.penetration > 0.0f);
}

TEST_CASE("Sphere vs AABB - 3D No Collision") {
    glm::vec3 spherePos(5, 5, 5);
    float radius = 1.0f;

    glm::vec3 boxPos(0, 0, 0);
    auto box = MakeBox(glm::vec3(1, 1, 1));

    auto result = SphereAABB(spherePos, radius, boxPos, box);

    REQUIRE_FALSE(result.collided);
}
