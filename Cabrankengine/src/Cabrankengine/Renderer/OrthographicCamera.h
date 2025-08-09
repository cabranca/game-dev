#pragma once

#include <glm/glm.hpp>

namespace cabrankengine {

	// Class representing an orthographic camera.
	// An orthographic camera is a type of camera that uses orthographic projection,
	// which means that it does not apply perspective distortion to the objects in the scene.
	class OrthographicCamera {
		public:
			// Constructor that initializes the orthographic camera with the specified left, right, bottom, and top clipping planes.
			OrthographicCamera(float left, float right, float bottom, float top);

			// Getters and setters for the camera's position and rotation.
			const glm::vec3& getPosition() const { return m_Position; }
			void setPosition(const glm::vec3& position) { m_Position = position; recalculateViewMatrix(); }
			float getRotation() const { return m_Rotation; }
			void setRotation(float rotation) { m_Rotation = rotation; recalculateViewMatrix(); }

			// Getters for the camera's projection, view, and view-projection matrices.
			const glm::mat4& getProjectionMatrix() const { return m_ProjectionMatrix; }
			const glm::mat4& getViewMatrix() const { return m_ViewMatrix; }
			const glm::mat4& getViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		private:
			// Recalculates the view matrix based on the current position and rotation of the camera.
			void recalculateViewMatrix();

			glm::mat4 m_ProjectionMatrix; // The projection matrix defines how the 3D scene is projected onto the 2D screen.
			glm::mat4 m_ViewMatrix; // The view matrix transforms the world coordinates into camera coordinates.
			glm::mat4 m_ViewProjectionMatrix; // The view-projection matrix combines the view and projection matrices for rendering.

			glm::vec3 m_Position; // The position of the camera in the 3D world.
			float m_Rotation; // The rotation of the camera around its up axis, in degrees.
	};
}
