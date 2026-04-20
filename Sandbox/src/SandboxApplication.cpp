#include <Cabrankengine.h>

// --- Entry Point ---
#include "Cabrankengine/Core/EntryPoint.h"

#include <Cabrankengine/Core/Core.h>
#include <Cabrankengine/Math/MatrixFactory.h>
#include <Cabrankengine/Math/Transform.h>
#include <Cabrankengine/Renderer/Materials/PBRMaterial.h>
#include <Cabrankengine/Renderer/Materials/PhongMaterial.h>
#include <Cabrankengine/Renderer/Shader.h>
#include <Cabrankengine/Scene/Model.h>
#include <imgui.h>
#include "Sandbox2D.h"

using namespace cbk;
using namespace cbk::math;
using namespace cbk::rendering;
using namespace cbk::scene;

#ifdef CBK_RENDERER_METAL

class ExampleLayer : public Layer {
  public:
	ExampleLayer() : Layer("Example"), m_CameraController(PerspectiveCamera(PI / 4.f, 16.f / 9.f, 0.1f, 100.f)) {
		m_ShaderLibrary.load("assets/shaders/Triangle");

		// --- Vertex data: position (Float3) + color (Float4) ---
		float vertices[] = {
			// x      y      z       r     g     b     a
			-0.5f, -0.5f,  0.0f,   1.0f, 0.0f, 0.0f, 1.0f,  // bottom-left  (red)
			 0.5f, -0.5f,  0.0f,   0.0f, 1.0f, 0.0f, 1.0f,  // bottom-right (green)
			 0.0f,  0.5f,  0.0f,   0.0f, 0.0f, 1.0f, 1.0f,  // top-center   (blue)
		};

		auto vb = VertexBuffer::create(vertices, sizeof(vertices));
		vb->setLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		});

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

	void onImGuiRender() override {
	}

  private:
	CameraController m_CameraController;
	ShaderLibrary m_ShaderLibrary;
	Ref<VertexArray> m_VertexArray;
};
#endif

#ifdef CBK_RENDERER_OPENGL
class ExampleLayer : public Layer {
  public:
	ExampleLayer() : Layer("Example"), m_CameraController(PerspectiveCamera(PI / 4.f, 16.f / 9.f, 0.1f, 100.f)) {
		m_CameraController.getCamera().setWorldPosition(Vector3(0.f, 0.f, 10.f));

		auto pbrShader = cbk::rendering::Shader::create("assets/shaders/PBR");
		auto pbrMaterial = cbk::createRef<cbk::rendering::PBRMaterial>(pbrShader);
		m_GunModel = new cbk::scene::Model<cbk::rendering::PBRMaterial>("assets/models/gun/Cerberus_LP.cbkm", pbrMaterial);

		auto phongShader = cbk::rendering::Shader::create("assets/shaders/Phong");
		auto phongMaterial = cbk::createRef<cbk::rendering::PhongMaterial>(phongShader);
		m_PhongModel = new cbk::scene::Model<cbk::rendering::PhongMaterial>("assets/models/backpack/backpack.cbkm", phongMaterial);

		m_LightEnvironment.DirLight.direction = { 0.0f, -1.0f, 0.0f  }; 
		m_LightEnvironment.DirLight.radiance = { 0.5f, 0.5f, 0.5f };

		cbk::rendering::PointLight lamp;
		lamp.position = { 0.0f, 0.0f, 8.0f }; 

		lamp.radiance = Vector3(1.f);

		lamp.constant = 1.0f;
		lamp.linear = 0.09f;
		lamp.quadratic = 0.032f;

		m_LightEnvironment.PointLights.push_back(lamp);
	}

	void onUpdate(Timestep delta) override {

		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		RenderCommand::clear();

		m_CameraController.onUpdate(delta);

		const auto& camera = m_CameraController.getCamera();

		Renderer::beginScene(camera, m_LightEnvironment);

		m_PhongModel->draw();
		m_GunModel->draw(scaleUniform(0.05f) * rotateX(-90.f) * translation({2.f, 2.f, 2.f}));
		//Renderer::submit(m_PBRMaterial, m_Sphere, scaleUniform(5.f));
		
		Renderer::endScene();

		TextRenderer::beginScene(camera.getViewMatrix() * camera.getProjectionMatrix());
		TextRenderer::drawText("HOLA MUNDO!", Vector3(-0.5f, 3.f, 0.f), 0.05f, Vector4(1.f));
		TextRenderer::endScene();
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
	LightEnvironment m_LightEnvironment;
	CameraController m_CameraController;
	ShaderLibrary m_ShaderLibrary;
	Ref<VertexArray> m_Sphere = nullptr;
	Model<PhongMaterial>* m_PhongModel;
	Model<PBRMaterial>* m_GunModel;
};
#endif

class Sandbox : public Application {
  public:
	Sandbox() {
		pushLayer(new ExampleLayer());
		//pushLayer(new Sandbox2D());
	}
	~Sandbox() {}
};

Application* cbk::createApplication() {
	return new Sandbox();
}
