#pragma once

#include <Cabrankengine/ECS/Common.h>
#include <Cabrankengine/ECS/Components.h>

namespace cbk::scene::arch {

	class DirectionalLightArch {
	  public:
		DirectionalLightArch();

		ecs::CDirectionalLight& light();

	  private:
		ecs::Entity m_Entity;
	};
} // namespace cbk::scene::arch
