#pragma once

#include <glm/glm.hpp>

namespace cabrankengine::components {

	struct Transform {
		glm::vec3 Position{ 0.0f };
		glm::vec3 Rotation{ 0.0f };
		glm::vec3 Scale{ 1.0f };
	};

	// Centered box
	struct CollisionBox {
		glm::vec3 HalfExtents{ 0.5f };
	};
}
