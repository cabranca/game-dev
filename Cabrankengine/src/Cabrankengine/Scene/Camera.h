#pragma once

#include <Cabrankengine/Math/Transform.h>

namespace cbk::scene {

	class Camera {
	  public:
		explicit Camera(const math::Mat4& projectionMatrix, const math::Transform& transform = math::Transform()) noexcept;

		const math::Transform& getTransform() const noexcept;
		void setTransform(const math::Transform& transform) noexcept;
		void setTransform(const math::Vector3& position, const math::Vector3& rotation, const math::Vector3& scale) noexcept;
		math::Vector3 getWorldPosition() const;
		void setWorldPosition(const math::Vector3& newPos);
		math::Vector3 getWorldRotation() const;
		void setWorldRotation(const math::Vector3& newRotation);
		void setWorldRotationAndPosition(const math::Vector3& newRotation, const math::Vector3& newPos);
		
		const math::Mat4& getViewMatrix() const noexcept;
		const math::Mat4& getProjectionMatrix() const noexcept;
		const math::Mat4& getViewProjectionMatrix() const noexcept;


	  protected:
		math::Transform m_Transform; // Model matrix of the camera
		math::Mat4 m_ViewMatrix; // Inverse of the Transform. Conerts World Space into View Space
		math::Mat4 m_ProjectionMatrix; // Projection (orthographic or perspective) to transform from View Space to Clip Space
		math::Mat4 m_ViewProjectionMatrix; // View and projection matrices multiplied in CPU before compunting in the shader

	  private:
		// Recalculates the view matrix based on the current position and rotation of the camera.
		void recalculateViewMatrix();
	};

	class PerspectiveCamera : public Camera {
        public:
            PerspectiveCamera(float fovy, float aspect, float nearZ, float farZ, const math::Transform& transform = math::Transform()) 
				: Camera(math::perspective(fovy, aspect, nearZ, farZ), transform) {}
    };

	// Class representing an orthographic camera.
	// An orthographic camera is a type of camera that uses orthographic projection,
	// which means that it does not apply perspective distortion to the objects in the scene.
	class OrthographicCamera : public Camera {
		public:
			// Constructor that initializes the orthographic camera with the specified left, right, bottom, and top clipping planes.
			OrthographicCamera(float left, float right, float bottom, float top, const math::Transform& transform = math::Transform())
				: Camera(math::ortho(left, right, bottom, top, -1.f, 1.f), transform) {}

			void setProjection(float left, float right, float bottom, float top) {
				m_ProjectionMatrix = math::ortho(left, right, bottom, top, -1.f, 1.f);
				m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
			}
	};
} // namespace cbk