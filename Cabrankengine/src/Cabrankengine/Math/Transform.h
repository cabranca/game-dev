#pragma once

#include "MatrixFactory.h"

namespace cbk::math {

    struct Transform {
        Vector3 Position;
        Vector3 Rotation;
        Vector3 Scale;

        Transform() : Position(), Rotation(), Scale(1.f) {}
        Transform(Vector3 position, Vector3 rotation, Vector3 scale) : Position(position), Rotation(rotation), Scale(scale) {}

        Mat4 toMat4() const noexcept {
            return scaleXYZ(Scale) * rotateXYZ(Rotation) * translation(Position);
        }
    };

}