#pragma once

#include <Cabrankengine/ECS/Common.h>
#include <Cabrankengine/ECS/Components.h>

namespace cbk::scene::arch {

	class CameraArch {
	  public:
		CameraArch(ecs::ProjectionType type);

		ecs::CTransform& transform();
		ecs::CCamera& camera();

	  private:
		ecs::Entity m_Entity;
	};
} // namespace cbk::scene::arch
