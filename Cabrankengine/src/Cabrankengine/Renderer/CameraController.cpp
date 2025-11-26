#include "CameraController.h"

#include <Cabrankengine/Core/Input.h>

namespace cabrankengine::rendering {

	using namespace math;

    CameraController::CameraController(Camera camera) : m_Camera(std::move(camera)) {}
	
    void CameraController::onUpdate(Timestep delta) {
		Vector3 position = m_Camera.getWorldPosition();
		Vector3 rotation = m_Camera.getWorldRotation();

		if (Input::isKeyPressed(Key::A)) 
			position.x -= delta;
		else if (Input::isKeyPressed(Key::D))
			position.x += delta;
		if (Input::isKeyPressed(Key::S))
			position.y -= delta;
		else if (Input::isKeyPressed(Key::W))
			position.y += delta;

		if (Input::isKeyPressed(Key::E))
			rotation.z -= delta;
		else if (Input::isKeyPressed(Key::Q))
			rotation.z += delta;

		m_Camera.setWorldRotation(rotation);
		m_Camera.setWorldPosition(position);
    }

	const Camera& CameraController::getCamera() const noexcept {
		return m_Camera;
	}
} // namespace cabrankengine::rendering
