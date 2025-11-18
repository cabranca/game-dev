#pragma once

#include "Camera.h"

namespace cabrankengine {

    class PerspectiveCamera : public Camera {
        public:
            PerspectiveCamera(float fovy, float aspect, float nearZ, float farZ);
    };
}