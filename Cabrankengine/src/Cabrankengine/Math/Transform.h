#pragma once

#include "MatrixFactory.h"

namespace cabrankengine::math {

    struct Transform {
        Vector3 position;
        Vector3 rotation;
        Vector3 scale;

        Transform() : position(), rotation(), scale(1.f) {}
        Transform(Vector3 position, Vector3 rotation, Vector3 scale) : position(position), rotation(rotation), scale(scale) {}

        Mat4 toMat4() const noexcept {
            return scaleXYZ(scale) * rotateXYZ(rotation) * translation(position);
        }
    };

}