#include "Coords.h"

#include <Cabrankengine/Core/Application.h>
#include <Cabrankengine/Core/Window.h>

using namespace glm;

namespace cabrankengine::coords {
    
    vec2 viewportToNormalized(float x, float y) {
        float windowWidth = Application::get().getWindow().getWidth();
        float windowHeight = Application::get().getWindow().getHeight();

        float normalizedX = (2.0f * x) / windowWidth - 1.f; 
        float normalizedY = (-2.0f * y) / windowHeight + 1.f;

        return vec2(normalizedX, normalizedY);
    }

    vec2 viewportToWorld(float x, float y, const OrthographicCameraController& cameraController) {
        float windowWidth = Application::get().getWindow().getWidth();
        float windowHeight = Application::get().getWindow().getHeight();

        vec3 cameraPos = cameraController.getCamera().getPosition();
        float cameraZoom = cameraController.getZoomLevel();

        float worldX = (x + cameraPos.x - windowWidth / 2.f) / cameraZoom;
        float worldY = (-y + cameraPos.y + windowHeight / 2.f) / cameraZoom;
        
        return vec2(worldX, worldY);
    }

    vec2 worldToViewport(const vec3& pos, const OrthographicCameraController& cameraController) {
        float windowWidth = Application::get().getWindow().getWidth();
        float windowHeight = Application::get().getWindow().getHeight();

        vec3 cameraPos = cameraController.getCamera().getPosition();
        float cameraZoom = cameraController.getZoomLevel();

        float viewportX = (pos.x - cameraPos.x + windowWidth / 2.f) * cameraZoom;
        float viewportY = (-pos.y + cameraPos.y + windowHeight / 2.f) * cameraZoom;

        return vec2(viewportX, viewportY);
    }
}
