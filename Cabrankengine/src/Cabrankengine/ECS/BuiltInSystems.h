#pragma once

#include <Cabrankengine/Scene/Camera.h>

#include "Registry.hpp"
#include "SystemManager.hpp"

namespace cbk::ecs {

	class CameraSystem : public ISystem {
	  public:
		CameraSystem(Registry& reg);
		void update(Registry& registry, float dt) override;

		const Ref<scene::Camera>& getActiveCamera() const;

	  private:
		Ref<scene::Camera> m_ActiveCamera;
	};

	class SpriteRenderSystem : public ISystem {
	  public:
		SpriteRenderSystem(Registry& reg);
		void update(Registry& reg, float dt) override;
	};

	class PhongRenderSystem : public ISystem {
	  public:
		PhongRenderSystem(Registry& reg);
		void update(Registry& reg, float dt) override;
	};

	class PBRRenderSystem : public ISystem {
	  public:
		PBRRenderSystem(Registry& reg);
		void update(Registry& reg, float dt) override;
	};

	class TextRenderSystem : public ISystem {
	  public:
		TextRenderSystem(Registry& reg);
		void update(Registry& reg, float dt) override;
	};
} // namespace cbk::ecs
