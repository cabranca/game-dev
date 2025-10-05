#pragma once

#include <glm/fwd.hpp>

#include <Cabrankengine/Renderer/OrthographicCameraController.h>

namespace cabrankengine::coords {
    glm::vec2 viewportToNormalized(float x, float y);
    glm::vec2 viewportToWorld(float x, float y, const OrthographicCameraController& cameraController);
    glm::vec2 worldToViewport(const glm::vec3& pos, const OrthographicCameraController& cameraController);
}
