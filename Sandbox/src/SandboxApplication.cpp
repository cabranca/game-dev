#include <Cabrankengine.h>

// --- Entry Point ---
#include "Cabrankengine/Core/EntryPoint.h"

#include <imgui.h>
#include "Sandbox2D.h"

using namespace cabrankengine;
using namespace cabrankengine::math;
using namespace cabrankengine::rendering;
using namespace cabrankengine::scene;

class ExampleLayer : public Layer {
  public:
	ExampleLayer() : Layer("Example"), m_CameraController(PerspectiveCamera(PI / 4.f, 16.f / 9.f, 0.1f, 100.f)) {
		m_CameraController.getCamera().setWorldPosition(Vector3(0.f, 0.f, 10.f));
		// auto shader = m_ShaderLibrary.load("assets/shaders/Lightning.glsl");
		// auto phongMaterial = std::make_shared<PhongMaterial>(shader);
		// m_ModelTest = new Model("assets/models/backpack/backpack.obj", phongMaterial);

		auto pbrShader = Shader::create("assets/shaders/PBR.glsl");
		m_PBRMaterial = std::make_shared<PBRMaterial>(pbrShader);
		m_Sphere = DefaultLibrary::getSphere();

		m_LightEnvironment.DirLight.direction = { 0.0f, -1.0f, 0.0f  }; 
		m_LightEnvironment.DirLight.radiance = { 0.5f, 0.5f, 0.5f };

		cabrankengine::rendering::PointLight lamp;
		lamp.position = { 0.0f, 0.0f, 8.0f }; 

		lamp.radiance = Vector3(1.f);

		lamp.constant = 1.0f;
		lamp.linear = 0.09f;
		lamp.quadratic = 0.032f;

		m_LightEnvironment.PointLights.push_back(lamp);

		// auto lightSource = m_ShaderLibrary.load("assets/shaders/LightSource.glsl");
		// m_Cube = std::make_shared<CubeMesh>(lightSource);
	}

	void onUpdate(Timestep delta) override {

		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		RenderCommand::clear();

		m_CameraController.onUpdate(delta);

		const auto& camera = m_CameraController.getCamera();

		Renderer::beginScene(camera, m_LightEnvironment);

		//m_ModelTest->draw();

		// auto lightSourceShader = m_ShaderLibrary.get("LightSource");
		// lightSourceShader->bind();
		// for (const auto& pointLight : m_LightEnvironment.PointLights) {
		// 	lightSourceShader->setFloat3("debugColor", pointLight.radiance);
		// 	m_Cube->draw(translation(pointLight.position));
		// }

		// auto errorShader = DefaultLibrary::getErrorShader();
		//auto cubeMesh = DefaultLibrary::getCube();
		Renderer::submit(m_PBRMaterial, m_Sphere, scaleUniform(5.f));
		
		Renderer::endScene();

		TextRenderer::beginScene(camera.getViewMatrix() * camera.getProjectionMatrix());
		TextRenderer::drawText("HOLA MUNDO!", Vector3(-0.5f, 3.f, 0.f), 0.05f, Vector4(1.f));
		TextRenderer::endScene();
	}

	void onImGuiRender() {
		CE_PROFILE_FUNCTION();
		ImGui::Begin("Settings");

		auto albedo = m_PBRMaterial->getAlbedoColor();
		auto metalness = m_PBRMaterial->getMetalness();
		auto roughness = m_PBRMaterial->getRoughness();

		ImGui::ColorEdit3("Albedo", &albedo.x);
		ImGui::InputFloat("Metalness", &metalness);
		ImGui::InputFloat("Roughness", &roughness);

		m_PBRMaterial->setAlbedoColor(albedo);
		m_PBRMaterial->setMetalness(metalness);
		m_PBRMaterial->setRoughness(roughness);

		ImGui::End();
	}

  private:
	LightEnvironment m_LightEnvironment;
	CameraController m_CameraController;
	ShaderLibrary m_ShaderLibrary;
	Model* m_ModelTest = nullptr;
	Ref<VertexArray> m_Cube = nullptr;
	Ref<PBRMaterial> m_PBRMaterial = nullptr;
	Ref<VertexArray> m_Sphere = nullptr;
};

class Sandbox : public Application {
  public:
	Sandbox() {
		pushLayer(new ExampleLayer());
		//pushLayer(new Sandbox2D());
	}
	~Sandbox() {}
};

Application* cabrankengine::createApplication() {
	return new Sandbox();
}
