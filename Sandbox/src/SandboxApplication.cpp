#include <imgui.h>

#include <Cabrankengine.h>

// --- Entry Point ---
#include "Cabrankengine/Core/EntryPoint.h"


#include "Sandbox2D.h"

using namespace cbk;
using namespace cbk::ecs;
using namespace cbk::math;
using namespace cbk::rendering;
using namespace cbk::scene;
using namespace cbk::scene::arch;

#ifdef CBK_RENDERER_METAL

class ExampleLayer : public Layer {
  public:
	ExampleLayer() : Layer("Example"), m_CameraController(PerspectiveCamera(PI / 4.f, 16.f / 9.f, 0.1f, 100.f)) {
		m_ShaderLibrary.load("assets/shaders/Triangle");

		// --- Vertex data: position (Float3) + color (Float4) ---
		float vertices[] = {
			// x      y      z       r     g     b     a
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // bottom-left  (red)
			0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // bottom-right (green)
			0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-center   (blue)
		};

		auto vb = VertexBuffer::create(vertices, sizeof(vertices));
		vb->setLayout({ { ShaderDataType::Float3, "a_Position" }, { ShaderDataType::Float4, "a_Color" } });

		uint32_t indices[] = { 0, 1, 2 };
		auto ib = IndexBuffer::create(indices, 3);

		m_VertexArray = VertexArray::create();
		m_VertexArray->addVertexBuffer(vb);
		m_VertexArray->setIndexBuffer(ib);
	}

	void onUpdate(Timestep delta) override {
		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		RenderCommand::clear();

		auto shader = m_ShaderLibrary.get("Triangle");
		Renderer::submit(shader, m_VertexArray, identityMat());
		Renderer::endScene();
	}

	void onImGuiRender() override {}

  private:
	CameraController m_CameraController;
	ShaderLibrary m_ShaderLibrary;
	Ref<VertexArray> m_VertexArray;
};
#endif

#ifdef CBK_RENDERER_OPENGL
class ExampleLayer : public Layer {
  public:
	ExampleLayer(const Ref<Registry>& reg) : Layer("Example"), m_CameraController(PerspectiveCamera(PI / 4.f, 16.f / 9.f, 0.1f, 100.f)) {
		m_CameraController.getCamera()->setWorldPosition(Vector3(0.f, 0.f, 10.f));
		m_Registry = reg;

		Entity camera = m_Registry->createEntity();
		m_Registry->addComponent(camera, CCamera{ .Camera = m_CameraController.getCamera() });

		PBRModelArch gun{};
		gun.transform().Transform.Position = {2.f, -2.f, 2.f};
		gun.transform().Transform.Rotation = {-90.f, 0.f, 0.f};
		gun.transform().Transform.Scale = Vector3(0.1f);
		gun.model().Model = createRef<Model<PBRMaterial>>("assets/models/gun/Cerberus_LP.cbkm", cbk::createRef<cbk::rendering::PBRMaterial>());

		PhongModelArch backpack{};
		backpack.model().Model = createRef<Model<PhongMaterial>>("assets/models/backpack/backpack.cbkm", cbk::createRef<cbk::rendering::PhongMaterial>());

		TextArch text{};
		text.transform().Transform.Position = {5.f, 0.f, 0.f};
		text.text().Text = "ALLA LA ESTAN RENDERIZANDO";
		text.text().Color = {1.f, 1.f, 1.0f, 1.f};

		LightEnvironment env;
		env.DirLight.direction = { 0.0f, -1.0f, 0.0f };
		env.DirLight.radiance = { 0.5f, 0.5f, 0.5f };

		cbk::rendering::PointLight lamp{
			.position = { 0.0f, 0.0f, 8.0f },
			.radiance = Vector3(1.f),
			.constant = 1.0f,
			.linear = 0.09f,
			.quadratic = 0.032f,
		};

		env.PointLights.push_back(lamp);
		RenderLayer::setLightEnvironment(env);
	}

	void onUpdate(Timestep delta) override {
		m_CameraController.onUpdate(delta);
	}

	void onImGuiRender() override {
		CBK_PROFILE_FUNCTION();
		// ImGui::Begin("Settings");

		// auto albedo = m_PBRMaterial->getAlbedoColor();
		// auto metalness = m_PBRMaterial->getMetalness();
		// auto roughness = m_PBRMaterial->getRoughness();

		// ImGui::ColorEdit3("Albedo", &albedo.x);
		// ImGui::InputFloat("Metalness", &metalness);
		// ImGui::InputFloat("Roughness", &roughness);

		// m_PBRMaterial->setAlbedoColor(albedo);
		// m_PBRMaterial->setMetalness(metalness);
		// m_PBRMaterial->setRoughness(roughness);

		// ImGui::End();
	}

  private:
	CameraController m_CameraController;
	Ref<Registry> m_Registry;
};
#endif

class Sandbox : public Application {
  public:
	Sandbox() {
		pushLayer(new ExampleLayer(m_Registry));
		//pushLayer(new Sandbox2D(m_Registry));
	}
	~Sandbox() {}
};

Application* cbk::createApplication() {
	return new Sandbox();
}
