#pragma once

#include <Cabrankengine/ECS/Common.h>
#include <Cabrankengine/ECS/Components.h>

namespace cbk::scene::arch {

	class TextArch {
	  public:
		TextArch();

		ecs::CTransform& transform();
		ecs::CText& text();

	  private:
		ecs::Entity m_Entity;
	};
} // namespace cbk::scene::arch
