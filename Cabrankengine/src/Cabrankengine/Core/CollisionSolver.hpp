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
            const VecType& pos2, const CBoundingBox<VecType>& b2)
        {
            VecType delta = glm::abs(pos1 - pos2);
            VecType overlap = b1.halfSize + b2.halfSize - delta;

            for (int i = 0; i < int(VecType().length()); ++i) {
                if (overlap[i] <= 0.0f)
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

    }

}
