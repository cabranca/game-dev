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
using namespace cabrankengine::platform::opengl;

class ExampleLayer : public Layer {
  public:
	ExampleLayer() : Layer("Example"), m_CameraController(PerspectiveCamera(PI / 4.f, 16.f / 9.f, 0.1f, 100.f)) {
		m_CubeVA = VertexArray::create();

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

		m_WoodenBoxTexture = Texture2D::create("assets/textures/container2.png");
		m_WoodenBoxSpecularTexture = Texture2D::create("assets/textures/container2_specular.png");
		auto lightingShader = m_ShaderLibrary.load("assets/shaders/Lightning.glsl");
		m_ShaderLibrary.load("assets/shaders/LightSource.glsl");

		std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->bind();
		std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformInt("material.diffuse", 0);
		std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformInt("material.specular", 1);
		std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat3("material.specular", Vector3(0.5f, 0.5f, 0.5f));
		std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat1("material.shininess", 64.f);
		
		std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat3("dirLight.direction", Vector3(-0.2f, -1.0f, -0.3f));
		std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat3("dirLight.ambient", Vector3(0.05f));
		std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat3("dirLight.diffuse", Vector3(0.4f));
		std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat3("dirLight.specular", Vector3(0.5f));

		for (int i = 0; i < m_PointLightPositions.size(); i++) {
			std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat3("pointLights[" + std::to_string(i) + "].position", m_CameraController.getCamera().getWorldPosition());
			std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat3("pointLights[" + std::to_string(i) + "].ambient", Vector3(0.05f));
			std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat3("pointLights[" + std::to_string(i) + "].diffuse", Vector3(0.8f));
			std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat3("pointLights[" + std::to_string(i) + "].specular", Vector3(1.f));
			std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat1("pointLights[" + std::to_string(i) + "].constant", 1.f);
			std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat1("pointLights[" + std::to_string(i) + "].linear", 0.09f);
			std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat1("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
		}
	}

	void onUpdate(Timestep delta) override {

		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		RenderCommand::clear();

		m_CameraController.onUpdate(delta);

		Renderer::beginScene(m_CameraController.getCamera());
		auto lightingShader = m_ShaderLibrary.get("Lightning");
		lightingShader->bind();


		const auto cameraRotation = m_CameraController.getCamera().getWorldRotation() + Vector3(0.f, -90.f, 0.f);
		Vector3 cameraForward{};
		cameraForward.x = - cosf(radians(cameraRotation.y)) * cosf(radians(cameraRotation.x));
		cameraForward.y = - sinf(radians(cameraRotation.x));
		cameraForward.z = sinf(radians(cameraRotation.y)) * cosf(radians(cameraRotation.x));
		cameraForward.normalized();

		std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat3("viewPos", m_CameraController.getCamera().getWorldPosition());

		static float rotation = 0.f;
		rotation += delta * 30.f;
		
		m_WoodenBoxTexture->bind(0);
		m_WoodenBoxSpecularTexture->bind(1);

		for (int i = 0; i < 9; i++) {
			Renderer::submit(lightingShader, m_CubeVA, translation(m_CubeVertexPositions[i]));
		}

		auto lightSourceShader = m_ShaderLibrary.get("LightSource");
		for (const auto& lightPos : m_PointLightPositions)
			Renderer::submit(lightSourceShader, m_CubeVA, scaleUniform(0.2f) * translation(lightPos));
		
		Renderer::endScene();
	}

  private:
	Vector3 m_CubeVertexPositions[9] = {
		Vector3( 2.0f,  5.0f, -15.0f), 
		Vector3(-1.5f, -2.2f, -2.5f),  
		Vector3(-3.8f, -2.0f, -12.3f),  
		Vector3( 2.4f, -0.4f, -3.5f),  
		Vector3(-1.7f,  3.0f, -7.5f),  
		Vector3( 1.3f, -2.0f, -2.5f),  
		Vector3( 1.5f,  2.0f, -2.5f), 
		Vector3( 1.5f,  0.2f, -1.5f), 
		Vector3(-1.3f,  1.0f, -1.5f)  
	};

	std::array<Vector3, 4> m_PointLightPositions = {
		Vector3( 0.7f,  0.2f,  2.0f),
		Vector3( 2.3f, -3.3f, -4.0f),
		Vector3(-4.0f,  2.0f, -12.0f),
		Vector3( 0.0f,  0.0f, -3.0f)
	};

	CameraController m_CameraController;
	ShaderLibrary m_ShaderLibrary;
	Ref<VertexArray> m_CubeVA;
	Ref<VertexArray> m_LightVA;
	Vector3 m_SquareColor = { 0.2f, 0.3f, 0.8f }; // Color of the square
	Ref<Texture2D> m_WoodenBoxTexture;
	Ref<Texture2D> m_WoodenBoxSpecularTexture;
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
