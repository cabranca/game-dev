#include "CameraController.h"

#include <Cabrankengine/Core/Input.h>

namespace cabrankengine::rendering {

	using namespace math;

	CameraController::CameraController(Camera camera)
	    : m_Camera(std::move(camera)), m_LastMouseX(), m_LastMouseY(), m_RotationSpeed(5.f), m_TranslationSpeed(10.f),
	      m_CameraPos(m_Camera.getWorldPosition()), m_CameraRot(m_Camera.getWorldRotation() + Vector3(0.f, -90.f, 0.f)) {
		Input::setInputMode(true, true);
	}

	void CameraController::onUpdate(Timestep delta) {

		Vector3 forward{};
		forward.x = - cosf(radians(m_CameraRot.y)) * cosf(radians(m_CameraRot.x));
		forward.y = - sinf(radians(m_CameraRot.x));
		forward.z = sinf(radians(m_CameraRot.y)) * cosf(radians(m_CameraRot.x));
		forward.normalized();
		Vector3 right = cross(Vector3::Up, forward).normalize();
		Vector3 up = cross(forward, right).normalize();
		
		if (Input::isKeyPressed(Key::A))
			m_CameraPos += right * m_TranslationSpeed * delta;
		else if (Input::isKeyPressed(Key::D))
			m_CameraPos -= right * m_TranslationSpeed * delta;
		if (Input::isKeyPressed(Key::W))
			m_CameraPos += forward * m_TranslationSpeed * delta;
		else if (Input::isKeyPressed(Key::S))
			m_CameraPos -= forward * m_TranslationSpeed * delta;
		if (Input::isKeyPressed(Key::E))
			m_CameraPos += up * m_TranslationSpeed * delta;
		else if (Input::isKeyPressed(Key::Q))
			m_CameraPos -= up * m_TranslationSpeed * delta;

		// if (Input::isKeyPressed(Key::E))
		// 	m_CameraRot.z -= 50.f * delta;
		// else if (Input::isKeyPressed(Key::Q))
		// 	m_CameraRot.z += 50.f * delta;

		auto [mouseX, mouseY] = Input::getMousePosition();
		if (m_LastMouseX != 0 ) {
			m_CameraRot.y += (m_LastMouseX - mouseX) * m_RotationSpeed * delta;
			//m_CameraRot.x += (m_LastMouseY - mouseY) * m_RotationSpeed * delta;
		}
		m_LastMouseX = mouseX;
		m_LastMouseY = mouseY;
		if (m_CameraRot.x > 89.0f)
			m_CameraRot.x = 89.0f;
		if (m_CameraRot.x < -89.0f)
			m_CameraRot.x = -89.0f;

		m_Camera.setWorldRotationAndPosition(m_CameraRot + Vector3(0.f, 90.f, 0.f), m_CameraPos);
	}

	const Camera& CameraController::getCamera() const noexcept {
		return m_Camera;
	}

	Camera& CameraController::getCamera() noexcept {
		return m_Camera;
	}
} // namespace cabrankengine::rendering
