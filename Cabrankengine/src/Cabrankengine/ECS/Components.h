#pragma once

#include <glm/glm.hpp>

namespace cabrankengine::components {

	struct CTransform {
		glm::vec3 Position{ 0.0f };
		glm::vec3 Rotation{ 0.0f };
		glm::vec3 Scale{ 1.0f };
	};

	// Centered box
	template<typename VecType>
	struct CCollisionBox {
		VecType HalfExtents{ 0.5f };

		CCollisionBox(const VecType& size) : HalfExtents(size / 2.0f) {}
	};
}
