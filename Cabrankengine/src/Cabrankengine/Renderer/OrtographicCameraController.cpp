#include <Cabrankengine/Renderer/OrthographicCameraController.h>

#include <Cabrankengine/Core/Core.h>
#include <Cabrankengine/Core/Input.h>

namespace cabrankengine {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_ZoomLevel(1.0f), m_Rotation(rotation), m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel) {
	}

	void OrthographicCameraController::onUpdate(Timestep delta) {
		if (Input::isKeyPressed(Key::A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * delta;
		else if (Input::isKeyPressed(Key::D))
			m_CameraPosition.x += m_CameraTranslationSpeed * delta;
		if (Input::isKeyPressed(Key::S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * delta;
		else if (Input::isKeyPressed(Key::W))
			m_CameraPosition.y += m_CameraTranslationSpeed * delta;

		if (m_Rotation) {
			if (Input::isKeyPressed(Key::E))
				m_CameraRotation += m_CameraRotationSpeed * delta;
			else if (Input::isKeyPressed(Key::Q))
				m_CameraRotation -= m_CameraRotationSpeed * delta;

			m_Camera.setRotation(m_CameraRotation);
		}

		m_Camera.setPosition(m_CameraPosition);
	}

	void OrthographicCameraController::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FN(&OrthographicCameraController::onMouseScrolled, this));
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(&OrthographicCameraController::onWindowResized, this));
	}

	void OrthographicCameraController::setZoomLevel(float level) {
		m_ZoomLevel = level;
	}

	float OrthographicCameraController::getZoomLevel() const {
		return m_ZoomLevel;
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e) {
		m_ZoomLevel -= e.getYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e) {
		m_AspectRatio = static_cast<float>(e.getWidth()) / static_cast<float>(e.getHeight());
		m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}
