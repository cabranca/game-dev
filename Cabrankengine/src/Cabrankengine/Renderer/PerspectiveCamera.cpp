#include "PerspectiveCamera.h"

namespace cabrankengine {

	using namespace math;

	cabrankengine::PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float nearZ, float farZ)
	    : Camera(perspective(fovy, aspect, nearZ, farZ), identityMat()) {}
} // namespace cabrankengine
