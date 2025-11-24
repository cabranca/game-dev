#include "PerspectiveCamera.h"

namespace cabrankengine::rendering {

	using namespace math;

	PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float nearZ, float farZ)
	    : Camera(perspective(fovy, aspect, nearZ, farZ), identityMat()) {}
} // namespace cabrankengine
