#include "Sandbox2D.h"

#include <imgui.h>

using namespace cbk;
using namespace cbk::ecs;
using namespace cbk::math;
using namespace cbk::rendering;
using namespace cbk::scene;
using namespace cbk::scene::arch;

Sandbox2D::Sandbox2D(const Ref<Registry>& reg) : Layer("Sandbox2D"), m_Camera(-800.f, 800.f, -450.f, 450.f), m_Registry(reg) {}

void Sandbox2D::onAttach() {
	CBK_PROFILE_FUNCTION();

	Entity camera = m_Registry->createEntity();
	m_Registry->addComponent(camera, CCamera{ .Camera = createRef<Camera>(m_Camera) });

	SpriteArch box{};
	box.sprite().Texture = Texture2D::create("assets/textures/container2.cbkt");
	box.transform().Transform.Scale = Vector3(500.f, 500.f, 0.f);
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
