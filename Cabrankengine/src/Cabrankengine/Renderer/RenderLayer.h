#pragma once

#include <Cabrankengine/Core/Layer.h>
#include <Cabrankengine/ECS/BuiltInSystems.h>
#include <Cabrankengine/ECS/Registry.hpp>
#include <Cabrankengine/Renderer/Lights.h>

namespace cbk::rendering {

	class RenderLayer : public Layer {
	  public:
		RenderLayer();

		void onAttach() override;
		void onUpdate(Timestep dt) override;

		static void setRegistry(const Ref<ecs::Registry>& reg);
		static void setLightEnvironment(const LightEnvironment& light);

	  private:
		Ref<ecs::Registry> m_Registry = nullptr;
		Ref<ecs::CameraSystem> m_CameraSystem = nullptr;
		Ref<ecs::SpriteRenderSystem> m_SpriteRenderSystem = nullptr;
		Ref<ecs::PhongRenderSystem> m_PhongRenderSystem = nullptr;
		Ref<ecs::PBRRenderSystem> m_PBRRenderSystem = nullptr;
		Ref<ecs::TextRenderSystem> m_TextRenderSystem = nullptr;
		LightEnvironment m_Lights;

		inline static RenderLayer* s_Instance = nullptr;

		void loadRegistry();
	};
} // namespace cbk::rendering
