#pragma once

#include <Cabrankengine/Math/Vector3.h>

namespace cabrankengine::rendering {

	struct LightComponents {
		math::Vector3 ambient{ 1.f };
		math::Vector3 diffuse{ 1.f };
		math::Vector3 specular{ 1.f };
	};

	struct DirectionalLight {
		math::Vector3 direction{ 0.f, -1.f, 0.f };
		LightComponents lightComponents;
	};

	struct PointLight {
		math::Vector3 position;
		LightComponents lightComponents;
		
		// Standard Attenuation
		float constant{ 1.f };
		float linear{ 0.09f };
		float quadratic{ 0.032f };
	};

	struct LightEnvironment {
		DirectionalLight DirLight;
		std::vector<PointLight> PointLights;
	};
} // namespace cabrankengine::rendering