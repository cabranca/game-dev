#pragma once

#include <Cabrankengine/Core/Timestep.h>

#include "Camera.h"

namespace cbk::scene {

	// FPS Camera Controller.
	class CameraController {
	  public:
		CameraController(Camera camera);
		void onUpdate(Timestep delta);

		const Ref<Camera>& getCamera() const noexcept;
		Ref<Camera>& getCamera() noexcept;

	  private:
		Ref<Camera> m_Camera;
		math::Vector3 m_CameraPos;
		math::Vector3 m_CameraRot;
		float m_LastMouseX, m_LastMouseY;
		float m_MouseSensitivity, m_TranslationSpeed;
		float m_Yaw, m_Pitch;
		bool m_MouseCaptured;
	};
} // namespace cbk::scene