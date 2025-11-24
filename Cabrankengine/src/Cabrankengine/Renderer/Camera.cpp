#include "Camera.h"

namespace cabrankengine::rendering {

	Camera::Camera(const math::Mat4& projectionMatrix, const math::Mat4& viewMatrix) noexcept
	    : m_Transform(math::identityMat()), m_ProjectionMatrix(projectionMatrix), m_ViewMatrix(viewMatrix),
	      m_ViewProjectionMatrix(m_ProjectionMatrix) {}

	const math::Mat4& Camera::getTransform() const noexcept {
		return m_Transform;
	}

	const void Camera::setTransform(const math::Mat4& transform) noexcept {
		m_Transform = transform;
		m_ViewProjectionMatrix = m_ProjectionMatrix * inverseAffine(m_Transform);
	}

	const void Camera::setTransform(const math::Vector3& position, const math::Vector3& rotation, const math::Vector3& scale) noexcept {
		m_Transform = math::translate(position) * math::rotate(rotation) * math::scale(scale);
		m_ViewProjectionMatrix = m_ProjectionMatrix * inverseAffine(m_Transform);
	}

	const math::Mat4& Camera::getViewProjectionMatrix() const noexcept {
		return m_ViewProjectionMatrix;
	}
} // namespace cabrankengine
