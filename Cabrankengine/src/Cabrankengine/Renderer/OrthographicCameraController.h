#pragma once

#include <Cabrankengine/Core/Timestep.h>
#include <Cabrankengine/Events/ApplicationEvent.h>
#include <Cabrankengine/Events/MouseEvent.h>
#include <Cabrankengine/Math/Vector3.h>

#include "OrthographicCamera.h"

namespace cabrankengine {

	class OrthographicCameraController {
		public:
			OrthographicCameraController(float width, float height, bool rotation = false);

			void onUpdate(Timestep delta);

			void onEvent(Event& e);

			float getZoomLevel() const;

			const OrthographicCamera& getCamera() const { return m_Camera; }
			OrthographicCamera& getCamera() { return m_Camera; }
		
		private:
			bool onMouseScrolled(MouseScrolledEvent& e);
			bool onWindowResized(WindowResizeEvent& e);

			float m_AspectRatio;
			float m_ZoomLevel;
			bool m_Rotation; // ROLL
			OrthographicCamera m_Camera;
			math::Vector3 m_CameraPosition;
			float m_CameraTranslationSpeed = 100.0f; // Speed of the camera movement
			float m_CurrentCameraTranslationSpeed = m_CameraTranslationSpeed;
			float m_CameraRotation = 0.0f; // Rotation of the camera
			float m_CameraRotationSpeed = 180.0f; // Speed of the camera rotation in degrees per second
			float m_Width;
			float m_Height;
	};
}
