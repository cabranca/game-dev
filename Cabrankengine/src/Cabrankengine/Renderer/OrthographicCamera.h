#pragma once

#include "Camera.h"

namespace cabrankengine::rendering {

	// Class representing an orthographic camera.
	// An orthographic camera is a type of camera that uses orthographic projection,
	// which means that it does not apply perspective distortion to the objects in the scene.
	class OrthographicCamera : public Camera {
		public:
			// Constructor that initializes the orthographic camera with the specified left, right, bottom, and top clipping planes.
			OrthographicCamera(float left, float right, float bottom, float top);

			void setProjection(float left, float right, float bottom, float top);

			// Getters and setters for the camera's position and rotation.
			const math::Vector3& getPosition() const { return m_Position; }
			void setPosition(const math::Vector3& position) { m_Position = position; recalculateViewMatrix(); }
			float getRotation() const { return m_Rotation; }
			void setRotation(float rotation) { m_Rotation = rotation; recalculateViewMatrix(); }

		private:
			// Recalculates the view matrix based on the current position and rotation of the camera.
			void recalculateViewMatrix();

			math::Vector3 m_Position; // The position of the camera in the 3D world.
			float m_Rotation; // The rotation of the camera around its up axis, in degrees.
	};
}
