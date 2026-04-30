#pragma once

#include <Cabrankengine/Core/Layer.h>
#include <Cabrankengine/ECS/BuiltInSystems.h>
#include <Cabrankengine/ECS/Registry.hpp>

namespace cbk::rendering {

	class RenderLayer : public Layer {
	  public:
		RenderLayer();

		void onAttach() override;
		void onUpdate(Timestep dt) override;
		void onEvent(Event& event) override;

		static void setRegistry(const Ref<ecs::Registry>& reg);

	  private:
		Ref<ecs::Registry> m_Registry = nullptr;
		Ref<ecs::CameraSystem> m_CameraSystem = nullptr;
		Ref<ecs::CameraControllerSystem> m_CameraControllerSystem = nullptr;
		Ref<ecs::DirectionalLightSystem> m_DirLightSystem = nullptr;
		Ref<ecs::PointLightSystem> m_PointLightSystem = nullptr;
		Ref<ecs::SpriteRenderSystem> m_SpriteRenderSystem = nullptr;
		Ref<ecs::PhongRenderSystem> m_PhongRenderSystem = nullptr;
		Ref<ecs::PBRRenderSystem> m_PBRRenderSystem = nullptr;
		Ref<ecs::TextRenderSystem> m_TextRenderSystem = nullptr;

		inline static RenderLayer* s_Instance = nullptr;

		void loadRegistry();
		bool onWindowResize(WindowResizeEvent& event);
	};
} // namespace cbk::rendering
