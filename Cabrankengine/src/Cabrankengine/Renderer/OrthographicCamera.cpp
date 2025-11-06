#include "OrthographicCamera.h"

#include <Cabrankengine/Debug/Instrumentator.h>
#include <Cabrankengine/Math/MatrixFactory.h>

using namespace cabrankengine::math;

namespace cabrankengine {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(ortho(left, right, bottom, top, -1.f, 1.f)), m_ViewMatrix(identityMat()), 
		  m_ViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix), m_Position(), m_Rotation() {}

	void OrthographicCamera::setProjection(float left, float right, float bottom, float top) {
		CE_PROFILE_FUNCTION();

		m_ProjectionMatrix = ortho(left, right, bottom, top, -1.f, 1.f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::recalculateViewMatrix() {
		CE_PROFILE_FUNCTION();

		Mat4 translation = translate( m_Position);
		Mat4 rotation = rotateZ(m_Rotation);
		m_ViewMatrix = inverse(translation * rotation);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
