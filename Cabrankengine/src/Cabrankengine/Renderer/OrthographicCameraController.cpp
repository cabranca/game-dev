#include <Cabrankengine/Renderer/OrthographicCameraController.h>

#include <Cabrankengine/Core/Core.h>
#include <Cabrankengine/Core/Input.h>
#include <Cabrankengine/Debug/Instrumentator.h>

namespace cabrankengine {

	OrthographicCameraController::OrthographicCameraController(float width, float height, bool rotation)
		: m_AspectRatio(width / height), m_ZoomLevel(1.0f), m_Rotation(rotation), m_Camera(-width / 2.f, width / 2.f, -height / 2.f, height / 2.f), 
		m_CameraPosition(glm::vec3(0.f)), m_Width(width), m_Height(height) {
	}

	void OrthographicCameraController::onUpdate(Timestep delta) {
		CE_PROFILE_FUNCTION();

		if (Input::isKeyPressed(Key::A)) {
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CurrentCameraTranslationSpeed * delta;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CurrentCameraTranslationSpeed * delta;
		}
		else if (Input::isKeyPressed(Key::D)) {
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CurrentCameraTranslationSpeed * delta;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CurrentCameraTranslationSpeed * delta;
		}
		if (Input::isKeyPressed(Key::S)) {
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CurrentCameraTranslationSpeed * delta;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CurrentCameraTranslationSpeed * delta;
		}
		else if (Input::isKeyPressed(Key::W)) {
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CurrentCameraTranslationSpeed * delta;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CurrentCameraTranslationSpeed * delta;
		}

		if (m_Rotation) {
			if (Input::isKeyPressed(Key::E))
				m_CameraRotation += m_CameraRotationSpeed * delta;
			else if (Input::isKeyPressed(Key::Q))
				m_CameraRotation -= m_CameraRotationSpeed * delta;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.setRotation(m_CameraRotation);
		}

		m_Camera.setPosition(m_CameraPosition);

		m_CurrentCameraTranslationSpeed = m_CameraTranslationSpeed / m_ZoomLevel;
	}

	void OrthographicCameraController::onEvent(Event& e) {
		CE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FN(&OrthographicCameraController::onMouseScrolled, this));
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(&OrthographicCameraController::onWindowResized, this));
	}

	float OrthographicCameraController::getZoomLevel() const {
		return m_ZoomLevel;
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e) {
		CE_PROFILE_FUNCTION();

		m_ZoomLevel += e.getYOffset() * 0.25;
		m_ZoomLevel = std::clamp(m_ZoomLevel, 0.125f, 8.f);
		m_Camera.setProjection((-m_Width / 2) / m_ZoomLevel, (m_Width / 2) / m_ZoomLevel, (-m_Height / 2) / m_ZoomLevel, (m_Height / 2) / m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e) {
		CE_PROFILE_FUNCTION();

		m_Width = static_cast<float>(e.getWidth());
		m_Height = static_cast<float>(e.getHeight());
		m_Camera.setProjection((-m_Width / 2) / m_ZoomLevel, (m_Width / 2) / m_ZoomLevel, (-m_Height / 2) / m_ZoomLevel, (m_Height / 2) / m_ZoomLevel);
		return false;
	}
}
