#pragma once

#include <Cabrankengine/Math/Vector3.h>

namespace cabrankengine::ecs {

	struct CTransform {
		math::Vector3 Position{ 0.0f };
		math::Vector3 Rotation{ 0.0f };
		math::Vector3 Scale{ 1.0f };
	};

	// Centered box
    // TODO: add concept
	template<typename VecType>
	struct CCollisionBox {
		VecType HalfExtents{ 0.5f };

		CCollisionBox(const VecType& size) : HalfExtents(size / 2.0f) {}
	};
}
