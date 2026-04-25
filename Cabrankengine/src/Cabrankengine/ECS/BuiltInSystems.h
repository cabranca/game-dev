#pragma once

#include <Cabrankengine/Scene/Camera.h>

#include "Registry.hpp"
#include "SystemManager.hpp"

namespace cbk::ecs {

	class CameraSystem : public ISystem {
	  public:
		  void update(Registry& registry, float dt) override;

	  private:
		  Ref<scene::Camera> m_ActiveCamera;
	};
} // namespace cbk::ecs
