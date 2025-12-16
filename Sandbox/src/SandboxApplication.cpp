#include <Cabrankengine.h>

// --- Entry Point ---
#include "Cabrankengine/Core/EntryPoint.h"

#include <Cabrankengine/Events/KeyEvent.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include <imgui.h>
#include "Sandbox2D.h"

using namespace cabrankengine;
using namespace cabrankengine::math;
using namespace cabrankengine::rendering;
using namespace cabrankengine::scene;
using namespace cabrankengine::platform::opengl;

class ExampleLayer : public Layer {
  public:
	ExampleLayer() : Layer("Example"), m_CameraController(PerspectiveCamera(PI / 4.f, 16.f / 9.f, 0.1f, 100.f)) {
		m_CameraController.getCamera().setWorldPosition(Vector3(0.f, 0.f, 10.f));
		auto shader = m_ShaderLibrary.load("assets/shaders/Lightning.glsl");
		m_ModelTest = new Model("assets/models/backpack/backpack.obj", shader);

		m_LightEnvironment.DirLight.direction = { 0.0f, 0.0f, 0.0f  }; 
		m_LightEnvironment.DirLight.lightComponents.ambient = { 0.05f, 0.05f, 0.05f };   // Ambiente muy tenue
		m_LightEnvironment.DirLight.lightComponents.diffuse = { 0.0f, 0.0f, 0.0f };     // Color "Sol" (blanco cálido)
		m_LightEnvironment.DirLight.lightComponents.specular = { 0.0f, 0.0f, 0.0f  };

		cabrankengine::rendering::PointLight lamp;
		lamp.position = { 1.0f, 2.0f, 2.0f }; 

		lamp.lightComponents.ambient = { 0.1f, 0.1f, 0.1f };
		lamp.lightComponents.diffuse = { 0.5f, 0.5f, 0.5f };
		lamp.lightComponents.specular = { 0.8f, 0.8f, 0.8f };

		lamp.constant = 1.0f;
		lamp.linear = 0.09f;
		lamp.quadratic = 0.032f;

		m_LightEnvironment.PointLights.push_back(lamp);

		m_ShaderLibrary.load("assets/shaders/LightSource.glsl");
		setupCube();
	}

	void onUpdate(Timestep delta) override {

		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		RenderCommand::clear();

		m_CameraController.onUpdate(delta);

		const auto& camera = m_CameraController.getCamera();

		Renderer::beginScene(camera.getProjectionMatrix(), camera.getViewMatrix(), m_LightEnvironment);

		m_ModelTest->draw();

		auto lightSourceShader = m_ShaderLibrary.get("LightSource");
		lightSourceShader->bind();
		for (const auto& pointLight : m_LightEnvironment.PointLights) {
			lightSourceShader->setFloat3("debugColor", Vector3(0.f, 0.5f, 0.5f));
			Outliner::drawOutline(lightSourceShader, m_CubeVA, Vector4(1.f), translation(m_LightEnvironment.PointLights[0].position));
		}
		
		Renderer::endScene();

		TextRenderer::beginScene(camera.getViewProjectionMatrix());
		TextRenderer::drawText("HOLA MUNDO!", Vector3(-0.5f, 3.f, 0.f), 0.05f, Color(1.f, 0.f, 0.f, 1.f));
		TextRenderer::endScene();
	}

	void onImGuiRender() {
		CE_PROFILE_FUNCTION();
		ImGui::Begin("Settings");

		auto& lightSource = m_LightEnvironment.PointLights[0];

		ImGui::ColorEdit3("Light Ambient", &lightSource.lightComponents.ambient.x);
		ImGui::ColorEdit3("Light Diffuse", &lightSource.lightComponents.diffuse.x);
		ImGui::ColorEdit3("Light Specular", &lightSource.lightComponents.specular.x);

		ImGui::End();
	}

  private:
	LightEnvironment m_LightEnvironment;
	CameraController m_CameraController;
	ShaderLibrary m_ShaderLibrary;
	Model* m_ModelTest = nullptr;
	Ref<VertexArray> m_CubeVA;

	void setupCube() {
		m_CubeVA = VertexArray::create();

		const std::array<float, 5 * 36> cullingCube {
			// Back face
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
			// Front face
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
			// Left face
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
			// Right face
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
			0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
			// Bottom face
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
			0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
			// Top face
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left 
		};

		float lightningCubeVertices[8 * 36] = {
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVB = VertexBuffer::create(lightningCubeVertices, sizeof(lightningCubeVertices));

		squareVB->setLayout({ { ShaderDataType::Float3, "pos" }, { ShaderDataType::Float3, "normal" }, { ShaderDataType::Float2, "texCoords" } });

		m_CubeVA->addVertexBuffer(squareVB);

		uint32_t lightningCubeIndices[36];
		for (int i = 0; i < 36; i++) {
			lightningCubeIndices[i] = i;
		}

		Ref<IndexBuffer> squareIB = IndexBuffer::create(lightningCubeIndices, sizeof(lightningCubeIndices) / sizeof(uint32_t));

		m_CubeVA->setIndexBuffer(squareIB);
	}
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
