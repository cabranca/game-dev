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

		float lightningCubeVertices[6 * 36] = {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
 			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 
			 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 
			 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 
			-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
			 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
			 0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
			 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
			 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
			 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
		};

		Ref<VertexBuffer> squareVB = VertexBuffer::create(lightningCubeVertices, sizeof(lightningCubeVertices));

		squareVB->setLayout({ { ShaderDataType::Float3, "pos" }, { ShaderDataType::Float3, "normal" } });

		m_CubeVA->addVertexBuffer(squareVB);

		uint32_t lightningCubeIndices[36];
		for (int i = 0; i < 36; i++) {
			lightningCubeIndices[i] = i;
		}

		Ref<IndexBuffer> squareIB = IndexBuffer::create(lightningCubeIndices, sizeof(lightningCubeIndices) / sizeof(uint32_t));

		m_CubeVA->setIndexBuffer(squareIB);

		auto lightingShader = m_ShaderLibrary.load("assets/shaders/Lightning.glsl");

		std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->bind();
		std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat3("material.ambient", Vector3(1.0f, 0.5f, 0.31f));
		std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat3("material.diffuse", Vector3(1.0f, 0.5f, 0.31f));
		std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat3("material.specular", Vector3(0.5f, 0.5f, 0.5f));
		std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat1("material.shininess", 32.f);

		std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat3("light.position", Vector3());
		

	}

	void onUpdate(Timestep delta) override {

		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		RenderCommand::clear();

		m_CameraController.onUpdate(delta);

		Renderer::beginScene(m_CameraController.getCamera());
		auto lightingShader = m_ShaderLibrary.get("Lightning");
		lightingShader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat3("viewPos", m_CameraController.getCamera().getWorldPosition());

		static float rotation = 0.f;
		rotation += delta * 30.f;

		Vector3 lightColor;
        lightColor.x = sinf(rotation * 0.02f);
        lightColor.y = sinf(rotation * 0.07f);
        lightColor.z = sinf(rotation * 0.013f);
        Vector3 diffuseColor = lightColor   * 0.5f; // decrease the influence
        Vector3 ambientColor = diffuseColor * 0.2f; // low influence
        std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat3("light.ambient", ambientColor);
		std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat3("light.diffuse", diffuseColor);
		std::dynamic_pointer_cast<OpenGLShader>(lightingShader)->uploadUniformFloat3("light.specular", Vector3(1.f));
		
		

		for (int i = 0; i < 9; i++) {
			Renderer::submit(lightingShader, m_CubeVA, rotateXYZ(Vector3(rotation, rotation, 0.f)) * translation(cubeVertexPositions[i]));
		}
		
		Renderer::endScene();
	}

  private:
	Vector3 cubeVertexPositions[9] = {
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

	CameraController m_CameraController;
	ShaderLibrary m_ShaderLibrary;
	Ref<VertexArray> m_CubeVA;
	Ref<VertexArray> m_LightVA;
	Vector3 m_SquareColor = { 0.2f, 0.3f, 0.8f }; // Color of the square
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
