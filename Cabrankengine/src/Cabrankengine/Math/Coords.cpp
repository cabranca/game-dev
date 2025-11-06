#include "Coords.h"

#include <glm/glm.hpp>

#include <Cabrankengine/Core/Application.h>
#include <Cabrankengine/Core/Window.h>

#include "Vector3.h"

using namespace glm;

namespace cabrankengine::math {

	vec2 viewportToNormalized(float x, float y) {
		float windowWidth = static_cast<float>(Application::get().getWindow().getWidth());
		float windowHeight = static_cast<float>(Application::get().getWindow().getHeight());

		float normalizedX = (2.0f * x) / windowWidth - 1.f;
		float normalizedY = (-2.0f * y) / windowHeight + 1.f;

		return { normalizedX, normalizedY };
	}

	vec2 viewportToWorld(float x, float y, const OrthographicCameraController& cameraController) {
		float windowWidth = static_cast<float>(Application::get().getWindow().getWidth());
		float windowHeight = static_cast<float>(Application::get().getWindow().getHeight());

		Vector3 cameraPos = cameraController.getCamera().getPosition();
		float cameraZoom = cameraController.getZoomLevel();

		float worldX = (x + cameraPos.x - windowWidth / 2.f) / cameraZoom;
		float worldY = (-y + cameraPos.y + windowHeight / 2.f) / cameraZoom;

		return { worldX, worldY };
	}

	vec2 worldToViewport(const vec3& pos, const OrthographicCameraController& cameraController) {
		float windowWidth = static_cast<float>(Application::get().getWindow().getWidth());
		float windowHeight = static_cast<float>(Application::get().getWindow().getHeight());

		Vector3 cameraPos = cameraController.getCamera().getPosition();
		float cameraZoom = cameraController.getZoomLevel();

		float viewportX = (pos.x - cameraPos.x + windowWidth / 2.f) * cameraZoom;
		float viewportY = (-pos.y + cameraPos.y + windowHeight / 2.f) * cameraZoom;

		return { viewportX, viewportY };
	}
} // namespace cabrankengine::coords
