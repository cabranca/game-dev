#pragma once

#include "Camera.h"

namespace cabrankengine::rendering {

    class PerspectiveCamera : public Camera {
        public:
            PerspectiveCamera(float fovy, float aspect, float nearZ, float farZ);
    };
}