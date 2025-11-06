#pragma once

#include <glm/glm.hpp>

#include <Cabrankengine/Renderer/OrthographicCameraController.h>

namespace cabrankengine::math {

	glm::vec2 viewportToNormalized(float x, float y);
	glm::vec2 viewportToWorld(float x, float y, const OrthographicCameraController& cameraController);
	glm::vec2 worldToViewport(const Vector3& pos, const OrthographicCameraController& cameraController);
} // namespace cabrankengine::math
