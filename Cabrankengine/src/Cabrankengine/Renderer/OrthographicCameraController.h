#pragma once

#include <Cabrankengine/Core/Timestep.h>
#include <Cabrankengine/Events/ApplicationEvent.h>
#include <Cabrankengine/Events/MouseEvent.h>
#include <Cabrankengine/Renderer/OrthographicCamera.h>

namespace cabrankengine {

	class OrthographicCameraController {
		public:
			OrthographicCameraController(float aspectRatio, bool rotation = false);

			void onUpdate(Timestep delta);

			void onEvent(Event& e);

			void setZoomLevel(float level);

			float getZoomLevel() const;

			const OrthographicCamera& getCamera() const { return m_Camera; }
			OrthographicCamera& getCamera() { return m_Camera; }
		
		private:
			bool onMouseScrolled(MouseScrolledEvent& e);
			bool onWindowResized(WindowResizeEvent& e);

			float m_AspectRatio;
			float m_ZoomLevel;
			bool m_Rotation;
			OrthographicCamera m_Camera;
			glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
			float m_CameraTranslationSpeed = 1.0f; // Speed of the camera movement
			float m_CameraRotation = 0.0f; // Rotation of the camera
			float m_CameraRotationSpeed = 90.0f; // Speed of the camera rotation in degrees per second
	};
}
