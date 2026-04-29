#pragma once

#include <Cabrankengine/ECS/Common.h>
#include <Cabrankengine/ECS/Components.h>

namespace cbk::scene::arch {

	class SpriteArch {
	  public:
		SpriteArch();

		ecs::CTransform& transform();
		ecs::CSprite& sprite();

	  private:
		ecs::Entity m_Entity;
	};
} // namespace cbk::scene::arch
