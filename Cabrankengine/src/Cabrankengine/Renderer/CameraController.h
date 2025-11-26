#pragma once

#include <Cabrankengine/Core/Timestep.h>

#include "Camera.h"

namespace cabrankengine::rendering {

    class CameraController {
      public:
        CameraController(Camera camera);
        void onUpdate(Timestep delta);

        const Camera& getCamera() const noexcept;

      private:
        Camera m_Camera;
    };
}