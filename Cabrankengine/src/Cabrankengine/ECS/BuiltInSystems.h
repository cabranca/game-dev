#pragma once

#include <Cabrankengine/Scene/Camera.h>

#include "Registry.hpp"
#include "SystemManager.hpp"

namespace cbk::ecs {

	class CameraSystem : public ISystem {
	  public:
		void update(Registry& registry, float dt) override;

		const Ref<scene::Camera>& getActiveCamera() const;

	  private:
		Ref<scene::Camera> m_ActiveCamera;
	};

	class SpriteRenderSystem : public ISystem {
	  public:
		void update(Registry& reg, float dt) override;
	};

	class PhongRenderSystem : public ISystem {
	  public:
		void update(Registry& reg, float dt) override;
	};

	class PBRRenderSystem : public ISystem {
	  public:
		void update(Registry& reg, float dt) override;
	};

	class TextRenderSystem : public ISystem {
	  public:
		void update(Registry& reg, float dt) override;
	};
} // namespace cbk::ecs
