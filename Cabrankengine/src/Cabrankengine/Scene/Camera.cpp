#include <pch.h>
#include "Camera.h"

namespace cbk::scene {

	using namespace math;

	Camera::Camera(const Mat4& projectionMatrix, const Transform& transform) noexcept
	    : m_Transform(transform), m_ProjectionMatrix(projectionMatrix), m_ViewMatrix(), m_ViewProjectionMatrix(m_ProjectionMatrix) {
		recalculateViewMatrix();
	}

	const Transform& Camera::getTransform() const noexcept {
		return m_Transform;
	}

	void Camera::setTransform(const Transform& transform) noexcept {
		m_Transform = transform;
		recalculateViewMatrix();
	}

	void Camera::setTransform(const Vector3& pos, const Vector3& rot, const Vector3& scale) noexcept {
		m_Transform = Transform(pos, rot, scale);
		recalculateViewMatrix();
	}

	math::Vector3 Camera::getWorldPosition() const {
		return m_Transform.position;
	}

	void Camera::setWorldPosition(const math::Vector3& newPos) {
		m_Transform.position = newPos;
		recalculateViewMatrix();
	}

	math::Vector3 Camera::getWorldRotation() const {
		return m_Transform.rotation;
	}

	void Camera::setWorldRotation(const math::Vector3& newRotation) {
		m_Transform.rotation = newRotation;
		recalculateViewMatrix();
	}

	void Camera::setWorldRotationAndPosition(const math::Vector3& newRotation, const math::Vector3& newPos) {
		m_Transform.rotation = newRotation;
		m_Transform.position = newPos;
		recalculateViewMatrix();
	}

	const Mat4& Camera::getViewMatrix() const noexcept {
		return m_ViewMatrix;
	}

	const Mat4& Camera::getProjectionMatrix() const noexcept {
		return m_ProjectionMatrix;
	}

	const Mat4& Camera::getViewProjectionMatrix() const noexcept {
		return m_ViewProjectionMatrix;
	}

	void Camera::recalculateViewMatrix() {
		m_ViewMatrix = inverseAffine(m_Transform.toMat4());
		m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
	}
} // namespace cbk
