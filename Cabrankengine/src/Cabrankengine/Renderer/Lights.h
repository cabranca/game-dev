#pragma once

#include <Cabrankengine/Math/Vector3.h>

namespace cbk::rendering {

	struct DirectionalLight {
		math::Vector3 direction{ 0.f, -1.f, 0.f };
		math::Vector3 radiance{ 1.f };
	};

	struct PointLight {
		math::Vector3 position;
		math::Vector3 radiance{ 1.f };
		
		// Standard Attenuation
		float constant{ 1.f };
		float linear{ 0.09f };
		float quadratic{ 0.032f };
	};

	struct LightEnvironment {
		DirectionalLight DirLight;
		std::vector<PointLight> PointLights;
	};
} // namespace cbk::rendering