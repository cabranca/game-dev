#include "Sandbox2D.h"

#include <chrono>
#include <imgui.h>

#include <Platform/OpenGL/OpenGLShader.h>

using namespace cabrankengine;
using namespace cabrankengine::ecs;
using namespace cabrankengine::math;
using namespace cabrankengine::rendering;
using namespace cabrankengine::scene;

class MovementSystem : public ISystem {
public:
	virtual void update(Registry& registry, float dt) override {
		for (const auto& e : m_Entities) {
			auto transform = registry.getComponent<CTransform>(e).value();
			
			transform->Position.x += 20.0f * dt;
			transform->Position.y += 20.0f * dt;
		}
	}
};

std::shared_ptr<MovementSystem> movSystem = nullptr;


Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_Camera(-800.f, 800.f, -450.f, 450.f), m_Registry(nullptr), m_Player() {}

void Sandbox2D::onAttach() {
	CE_PROFILE_FUNCTION();

	//m_CheckerboardTexture = Texture2D::create("assets/textures/Checkerboard.png");

	//AudioEngine::playAudio("assets/sound/breakout.mp3", true);

	m_Registry = new Registry();
	m_Registry->registerComponent<CTransform>();

	movSystem = m_Registry->registerSystem<MovementSystem>();

	Signature signature;
	signature.set(m_Registry->getComponentType<CTransform>(), true);
	m_Registry->setSystemSignature<MovementSystem>(signature);

	m_Player = m_Registry->createEntity();
	m_Registry->addComponent<CTransform>(m_Player, { {0,0,0}, {0,0,0}, {1,1,1} });
}

void Sandbox2D::onDetach() {
	CE_PROFILE_FUNCTION();
	delete m_Registry;
}

void Sandbox2D::onUpdate(cabrankengine::Timestep delta) {
	CE_PROFILE_FUNCTION();

	// Update
	movSystem->update(*m_Registry, delta);

	// Render
	Renderer2D::resetStats();
	{
		CE_PROFILE_SCOPE("Renderer Prep");
		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::clear();
	}

	{
		static float rotation = 0.f;
		rotation += delta * 90.f;

		CE_PROFILE_SCOPE("Renderer Draw");

		Renderer2D::beginScene(m_Camera.getViewProjectionMatrix());

		auto transform = m_Registry->getComponent<CTransform>(m_Player).value();
	    Renderer2D::drawQuad(transform->Position, { 100.0f, 100.0f }, m_SquareColor);
		Renderer2D::endScene();

		// TextRenderer::beginScene(m_Camera.getViewProjectionMatrix());
		// TextRenderer::drawText("HOLA MUNDO!", Vector3(0.f, 0.f, 0.1f), 1.f, Vector4(1.f));
		// TextRenderer::endScene();
	}
}

void Sandbox2D::onImGuiRender() {
	CE_PROFILE_FUNCTION();
}

void Sandbox2D::onEvent(cabrankengine::Event& e) {
	//m_Camera.onEvent(e);
}
