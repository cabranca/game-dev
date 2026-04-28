#include <pch.h>
#include "RenderLayer.h"

#include <Cabrankengine/ECS/Components.h>

#include "Renderer.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include "TextRenderer.h"

namespace cbk::rendering {

	using namespace ecs;

	RenderLayer::RenderLayer() {
		CBK_CORE_ASSERT(!s_Instance, "RenderLayer already exists!");
		s_Instance = this;
	}

	void RenderLayer::onAttach() {
		if (m_Registry)
			loadRegistry();
	}

	void RenderLayer::onUpdate(Timestep dt) {
		RenderCommand::setClearColor({ .2f, .2f, .2f, 1.f });
		RenderCommand::clear();

		m_CameraSystem->update(*m_Registry, dt);
		auto camera = m_CameraSystem->getActiveCamera();

		Renderer2D::beginScene(camera->getViewProjectionMatrix());
		m_SpriteRenderSystem->update(*m_Registry, dt);
		Renderer2D::endScene();

		Renderer::beginScene(*camera, m_Lights);
		m_PhongRenderSystem->update(*m_Registry, dt);
		m_PBRRenderSystem->update(*m_Registry, dt);
		Renderer::endScene();

		TextRenderer::beginScene(camera->getViewProjectionMatrix());
		m_TextRenderSystem->update(*m_Registry, dt);
		TextRenderer::endScene();
	}

	void RenderLayer::setRegistry(const Ref<Registry>& reg) {
		s_Instance->m_Registry = reg;
	}

	void RenderLayer::setLightEnvironment(const LightEnvironment& lights) {
		s_Instance->m_Lights = lights;
	}

	void RenderLayer::loadRegistry() {
		m_Registry->registerComponent<CTransform>();
		m_Registry->registerComponent<CCamera>();
		m_Registry->registerComponent<CSprite>();
		m_Registry->registerComponent<CPhongModel>();
		m_Registry->registerComponent<CPBRModel>();
		m_Registry->registerComponent<CText>();

		m_CameraSystem = m_Registry->registerSystem<CameraSystem>();
		m_SpriteRenderSystem = m_Registry->registerSystem<SpriteRenderSystem>();
		m_PhongRenderSystem = m_Registry->registerSystem<PhongRenderSystem>();
		m_PBRRenderSystem = m_Registry->registerSystem<PBRRenderSystem>();
		m_TextRenderSystem = m_Registry->registerSystem<TextRenderSystem>();
	}
} // namespace cbk::rendering
