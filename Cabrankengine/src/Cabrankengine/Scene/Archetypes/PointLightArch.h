#pragma once

#include <Cabrankengine/ECS/Common.h>
#include <Cabrankengine/ECS/Components.h>

namespace cbk::scene::arch {

	class PointLightArch {
	  public:
		PointLightArch();

		ecs::CTransform& transform();
		ecs::CPointLight& light();

	  private:
		ecs::Entity m_Entity;
	};
} // namespace cbk::scene::arch
