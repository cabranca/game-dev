#include "Camera.h"

namespace cabrankengine::rendering {

	using namespace math;

	Camera::Camera(const Mat4& projectionMatrix, const Mat4& viewMatrix) noexcept
	    : m_Transform(identityMat()), m_ProjectionMatrix(projectionMatrix), m_ViewMatrix(viewMatrix),
	      m_ViewProjectionMatrix(m_ProjectionMatrix) {}

	const Mat4& Camera::getTransform() const noexcept {
		return m_Transform;
	}

	void Camera::setTransform(const Mat4& transform) noexcept {
		m_Transform = transform;
		recalculateViewMatrix();
	}

	void Camera::setTransform(const Vector3& p, const Vector3& r, const Vector3& s) noexcept {
		m_Transform = translate(p) * rotate(r) * scale(s);
		recalculateViewMatrix();
	}

	math::Vector3 Camera::getWorldPosition() {
		const auto& translateVec = m_Transform.elements[3];
		return { translateVec.x, translateVec.y, translateVec.z };
	}

	void Camera::setWorldPosition(const math::Vector3& newPos) {
		auto& translateVec = m_Transform.elements[3];
		translateVec = newPos;
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
		m_ViewMatrix = inverseAffine(m_Transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
} // namespace cabrankengine
