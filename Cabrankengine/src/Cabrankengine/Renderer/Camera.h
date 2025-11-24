#pragma once

#include <Cabrankengine/Math/MatrixFactory.h>

namespace cabrankengine::rendering {

	class Camera {
	  public:
		Camera(const math::Mat4& projectionMatrix, const math::Mat4& ViewMatrix) noexcept;

		const math::Mat4& getTransform() const noexcept;
		const void setTransform(const math::Mat4& transform) noexcept;
		const void setTransform(const math::Vector3& position, const math::Vector3& rotation, const math::Vector3& scale) noexcept;
		const math::Mat4& getViewProjectionMatrix() const noexcept;

	  protected:
		math::Mat4 m_Transform;
		math::Mat4 m_ProjectionMatrix;
		math::Mat4 m_ViewMatrix;
		math::Mat4 m_ViewProjectionMatrix;
	};
} // namespace cabrankengine