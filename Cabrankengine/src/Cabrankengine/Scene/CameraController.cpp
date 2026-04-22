#include <pch.h>
#include "CameraController.h"

#include <Cabrankengine/Core/Input.h>

namespace cbk::scene {

	using namespace math;

	CameraController::CameraController(Camera camera)
	    : m_Camera(std::move(camera)), m_CameraPos(m_Camera.getWorldPosition()), m_CameraRot(m_Camera.getWorldRotation()), m_LastMouseX(), m_LastMouseY(),
	      m_MouseSensitivity(0.15f), m_TranslationSpeed(10.f), m_Yaw(m_Camera.getWorldRotation().y), m_Pitch(m_Camera.getWorldRotation().x), m_MouseCaptured(false) {
		auto [mouseX, mouseY] = Input::getMousePosition();
		m_LastMouseX = mouseX;
		m_LastMouseY = mouseY;
	}

	void CameraController::onUpdate(Timestep delta) {
		const float deltaSeconds = delta;
		auto [mouseX, mouseY] = Input::getMousePosition();
		const bool captureMouse = Input::isMouseButtonPressed(Mouse::ButtonRight);

		if (captureMouse) {
			Input::setInputMode(true, true);

			if (m_MouseCaptured) {
				const float deltaX = mouseX - m_LastMouseX;
				const float deltaY = mouseY - m_LastMouseY;

				m_Yaw -= deltaX * m_MouseSensitivity;
				m_Pitch -= deltaY * m_MouseSensitivity;
				m_Pitch = std::clamp(m_Pitch, -89.0f, 89.0f);
			}

			m_MouseCaptured = true;
		} else {
			Input::setInputMode(false, false);
			m_MouseCaptured = false;
		}

		m_LastMouseX = mouseX;
		m_LastMouseY = mouseY;

		const float yawRadians = radians(m_Yaw);
		const float pitchRadians = radians(m_Pitch);

		Vector3 forward{
			-std::sin(yawRadians) * std::cos(pitchRadians),
			std::sin(pitchRadians),
			-std::cos(yawRadians) * std::cos(pitchRadians)
		};
		forward.normalized();

		Vector3 flatForward{ forward.x, 0.f, forward.z };
		flatForward.normalized();

		if (flatForward.lengthSquared() == 0.f)
			flatForward = Vector3::Forward;

		const Vector3 flatRight = cross(flatForward, Vector3::Up).normalize();

		Vector3 movement{};

		if (Input::isKeyPressed(Key::W))
			movement += flatForward;
		if (Input::isKeyPressed(Key::S))
			movement -= flatForward;
		if (Input::isKeyPressed(Key::A))
			movement -= flatRight;
		if (Input::isKeyPressed(Key::D))
			movement += flatRight;
		if (Input::isKeyPressed(Key::Space))
			movement += Vector3::Up;
		if (Input::isKeyPressed(Key::LeftShift))
			movement += Vector3::Down;

		if (movement.lengthSquared() > 0.f) {
			movement.normalized();
			m_CameraPos += movement * m_TranslationSpeed * deltaSeconds;
		}

		m_CameraRot = { m_Pitch, m_Yaw, 0.f };
		m_Camera.setWorldRotationAndPosition(m_CameraRot, m_CameraPos);
	}

	const Camera& CameraController::getCamera() const noexcept {
		return m_Camera;
	}

	Camera& CameraController::getCamera() noexcept {
		return m_Camera;
	}
} // namespace cbk::rendering
