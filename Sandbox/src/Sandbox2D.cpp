#include "Sandbox2D.h"

#include <Cabrankengine/Core/Core.h>
#include <Cabrankengine/ECS/Components.h>
#include <Cabrankengine/Math/Vector3.h>
#include <Cabrankengine/Renderer/Texture.h>
#include <chrono>
#include <imgui.h>

#include <Platform/OpenGL/OpenGLShader.h>

using namespace cbk;
using namespace cbk::ecs;
using namespace cbk::math;
using namespace cbk::rendering;
using namespace cbk::scene;

Sandbox2D::Sandbox2D(const Ref<Registry>& reg) : Layer("Sandbox2D"), m_Camera(-800.f, 800.f, -450.f, 450.f), m_Registry(reg) {}

void Sandbox2D::onAttach() {
	CBK_PROFILE_FUNCTION();

	Entity camera = m_Registry->createEntity();
	m_Registry->addComponent(camera, CCamera{ .Camera = createRef<Camera>(m_Camera) });

	Transform spriteTransform;
	spriteTransform.Scale = Vector3(500.f, 500.f, 0.f);
	m_ConvertedTexture = Texture2D::create("assets/textures/container2.cbkt");
	auto box = m_Registry->createEntity();
	m_Registry->addComponent(box, CTransform(spriteTransform));
	m_Registry->addComponent(box, CSprite{ .Texture = m_ConvertedTexture });
}

void Sandbox2D::onDetach() {
	CBK_PROFILE_FUNCTION();
}

void Sandbox2D::onUpdate(cbk::Timestep delta) {
	CBK_PROFILE_FUNCTION();

	{
		static float rotation = 0.f;
		rotation += delta * 90.f;

		CBK_PROFILE_SCOPE("Renderer Draw");

		// TextRenderer::beginScene(m_Camera.getViewProjectionMatrix());
		// TextRenderer::drawText("HOLA MUNDO!", Vector3(0.f, 0.f, 0.1f), 1.f, Vector4(1.f));
		// TextRenderer::endScene();
	}
}

void Sandbox2D::onImGuiRender() {
	CBK_PROFILE_FUNCTION();
}

void Sandbox2D::onEvent(cbk::Event& e) {
	//m_Camera.onEvent(e);
}
