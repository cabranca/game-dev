#pragma once

#include <Cabrankengine/ECS/Common.h>
#include <Cabrankengine/ECS/Components.h>

namespace cbk::scene::arch {

	class PBRModelArch {
	  public:
		PBRModelArch();

		ecs::CTransform& transform();
		ecs::CPBRModel& model();

	  private:
		ecs::Entity m_Entity;
	};
} // namespace cbk::scene::arch
