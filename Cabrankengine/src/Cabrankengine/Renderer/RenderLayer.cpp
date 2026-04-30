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

		m_CameraControllerSystem->update(*m_Registry, dt);
		m_CameraSystem->update(*m_Registry, dt);

		Renderer2D::beginScene(m_CameraSystem->getViewProjectionMatrix());
		m_SpriteRenderSystem->update(*m_Registry, dt);
		Renderer2D::endScene();

		LightEnvironment lights;
		for (auto e : m_DirLightSystem->getEntities()) {
			auto dl = m_Registry->getComponent<ecs::CDirectionalLight>(e).value();
			lights.DirLight = { dl->Direction, dl->Radiance };
			break;
		}
		for (auto e : m_PointLightSystem->getEntities()) {
			auto transform = m_Registry->getComponent<ecs::CTransform>(e).value();
			auto pl = m_Registry->getComponent<ecs::CPointLight>(e).value();
			lights.PointLights.push_back({ transform->Position, pl->Radiance, pl->Constant, pl->Linear, pl->Quadratic });
		}

		Renderer::beginScene(m_CameraSystem->getViewProjectionMatrix(), m_CameraSystem->getCameraWorldPosition(), lights);
		m_PhongRenderSystem->update(*m_Registry, dt);
		m_PBRRenderSystem->update(*m_Registry, dt);
		Renderer::endScene();

		TextRenderer::beginScene(m_CameraSystem->getViewProjectionMatrix());
		m_TextRenderSystem->update(*m_Registry, dt);
		TextRenderer::endScene();
	}

	void RenderLayer::onEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(&RenderLayer::onWindowResize, this));
	}

	void RenderLayer::setRegistry(const Ref<Registry>& reg) {
		s_Instance->m_Registry = reg;
	}

	void RenderLayer::loadRegistry() {
		m_Registry->registerComponent<CTransform>();
		m_Registry->registerComponent<CCamera>();
		m_Registry->registerComponent<CCameraController>();
		m_Registry->registerComponent<CDirectionalLight>();
		m_Registry->registerComponent<CPointLight>();
		m_Registry->registerComponent<CSprite>();
		m_Registry->registerComponent<CPhongModel>();
		m_Registry->registerComponent<CPBRModel>();
		m_Registry->registerComponent<CText>();

		m_CameraSystem = m_Registry->registerSystem<CameraSystem>();
		m_CameraControllerSystem = m_Registry->registerSystem<CameraControllerSystem>();
		m_DirLightSystem = m_Registry->registerSystem<DirectionalLightSystem>();
		m_PointLightSystem = m_Registry->registerSystem<PointLightSystem>();
		m_SpriteRenderSystem = m_Registry->registerSystem<SpriteRenderSystem>();
		m_PhongRenderSystem = m_Registry->registerSystem<PhongRenderSystem>();
		m_PBRRenderSystem = m_Registry->registerSystem<PBRRenderSystem>();
		m_TextRenderSystem = m_Registry->registerSystem<TextRenderSystem>();

		Signature sig;
		sig.set(m_Registry->getComponentType<CTransform>());
		sig.set(m_Registry->getComponentType<CCamera>());
		m_Registry->setSystemSignature<CameraSystem>(sig);

		sig.reset();
		sig.set(m_Registry->getComponentType<CTransform>());
		sig.set(m_Registry->getComponentType<CCameraController>());
		m_Registry->setSystemSignature<CameraControllerSystem>(sig);

		sig.reset();
		sig.set(m_Registry->getComponentType<CDirectionalLight>());
		m_Registry->setSystemSignature<DirectionalLightSystem>(sig);

		sig.reset();
		sig.set(m_Registry->getComponentType<CTransform>());
		sig.set(m_Registry->getComponentType<CPointLight>());
		m_Registry->setSystemSignature<PointLightSystem>(sig);

		sig.reset();
		sig.set(m_Registry->getComponentType<CTransform>());
		sig.set(m_Registry->getComponentType<CSprite>());
		m_Registry->setSystemSignature<SpriteRenderSystem>(sig);

		sig.reset();
		sig.set(m_Registry->getComponentType<CTransform>());
		sig.set(m_Registry->getComponentType<CPhongModel>());
		m_Registry->setSystemSignature<PhongRenderSystem>(sig);

		sig.reset();
		sig.set(m_Registry->getComponentType<CTransform>());
		sig.set(m_Registry->getComponentType<CPBRModel>());
		m_Registry->setSystemSignature<PBRRenderSystem>(sig);

		sig.reset();
		sig.set(m_Registry->getComponentType<CTransform>());
		sig.set(m_Registry->getComponentType<CText>());
		m_Registry->setSystemSignature<TextRenderSystem>(sig);
	}

	bool RenderLayer::onWindowResize(WindowResizeEvent& event) {
		return m_CameraSystem->onWindowResize(event);
	}
} // namespace cbk::rendering
