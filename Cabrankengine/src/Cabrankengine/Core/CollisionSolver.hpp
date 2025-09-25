#pragma once

#include <glm/glm.hpp>
#include <vector>

#include <Cabrankengine/ECS/Components.hpp>

namespace cabrankengine {

    namespace collision_solver {

        template<typename VecType>
        struct Collision {
            VecType normal;     // Direction of highest penetration value from the first object to the second
            float penetration;  // How much the objects are colliding
            bool collided;      // True if there is collision
        };


        template<typename VecType>
        Collision<VecType> AABB(const VecType& pos1, const CBoundingBox<VecType>& b1,
            const VecType& pos2, const CBoundingBox<VecType>& b2) {
            VecType delta = glm::abs(pos1 - pos2);
            VecType overlap = b1.halfSize + b2.halfSize - delta;

            for (int i = 0; i < int(VecType().length()); ++i) {
                if (overlap[i] < 0.0f)
                    return { VecType(0.0f), 0.0f, false };
            }

            int minAxis = 0;
            float minOverlap = overlap[0];
            for (int i = 1; i < int(VecType().length()); ++i) {
                if (overlap[i] < minOverlap) {
                    minOverlap = overlap[i];
                    minAxis = i;
                }
            }

            VecType normal(0.0f);
            normal[minAxis] = (pos2[minAxis] > pos1[minAxis]) ? 1.0f : -1.0f;

            return { normal, minOverlap, true };
        }

        template<typename VecType>
        Collision<VecType> Sphere(const VecType& pos1, float radius1, const VecType& pos2, float radius2) {
            VecType delta = pos2 - pos1;
            float dist2 = glm::dot(delta, delta);
            float radiusSum = radius1 + radius2;

            if (dist2 > radiusSum * radiusSum) {
                return { VecType(0.0f), 0.0f, false };
            }

            float dist = glm::sqrt(dist2);

            // Avoid dividing by zero if the centers match
            VecType normal = (dist > 0.0001f) ? (delta / dist) : VecType(0.0f);

            float penetration = radiusSum - dist;

            return { normal, penetration, true };
        }

        template<typename VecType>
        Collision<VecType> SphereAABB(const VecType& spherePos, float radius, const VecType& boxPos, const CBoundingBox<VecType>& box) {
            // Calculate the closest point from the AABB to the sphere center
            VecType closestPoint;
            for (int i = 0; i < int(VecType().length()); ++i) {
                float min = boxPos[i] - box.halfSize[i];
                float max = boxPos[i] + box.halfSize[i];
                closestPoint[i] = glm::clamp(spherePos[i], min, max);
            }

            VecType delta = spherePos - closestPoint;
            float dist2 = glm::dot(delta, delta);

            if (dist2 > radius * radius) {
                return { VecType(0.0f), 0.0f, false };
            }

            float dist = glm::sqrt(dist2);

            // Collision Normal from AABB to Sphere
            VecType normal;
            if (dist > 0.0001f) {
                normal = delta / dist;
            }
            else {
                // Border case: sphere center inside AABB
                // We take the direction of minimum penetration, as in AABB-AABB
                VecType overlap = box.halfSize - glm::abs(spherePos - boxPos);
                int minAxis = 0;
                float minOverlap = overlap[0];
                for (int i = 1; i < int(VecType().length()); ++i) {
                    if (overlap[i] < minOverlap) {
                        minOverlap = overlap[i];
                        minAxis = i;
                    }
                }
                normal = VecType(0.0f);
                normal[minAxis] = (spherePos[minAxis] > boxPos[minAxis]) ? 1.0f : -1.0f;
                dist = 0.0f; // Already inside, no distance
            }

            float penetration = radius - dist;

            return { normal, penetration, true };
        }

    }

}
