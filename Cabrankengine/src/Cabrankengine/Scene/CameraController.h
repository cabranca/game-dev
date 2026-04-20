#pragma once

#include <Cabrankengine/Core/Timestep.h>

#include "Camera.h"

namespace cabrankengine::scene {

    // Actually a mix of FPS and free-fly Camera Controller.
    // Can move in forward, right and up directions. Can rotate yaw holding down the wheel.
    class CameraController {
      public:
        CameraController(Camera camera);
        void onUpdate(Timestep delta);

        const Camera& getCamera() const noexcept;
        Camera& getCamera() noexcept;

      private:
        Camera m_Camera;
        math::Vector3 m_CameraPos;
        math::Vector3 m_CameraRot; // It always has a -90° yaw offset due to OpenGL default orientation.
        float m_LastMouseX, m_LastMouseY;
        float m_RotationSpeed, m_TranslationSpeed;
    };
}