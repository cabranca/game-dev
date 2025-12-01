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
	ExampleLayer() : Layer("Example"), m_CameraController(PerspectiveCamera(-16.f / 9.f, 16.f / 9.f, 0.1f, 100.f)) {
		m_SquareVA = VertexArray::create();

		float squareVertices[5 * 36] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVB = VertexBuffer::create(squareVertices, sizeof(squareVertices));

		squareVB->setLayout({ { ShaderDataType::Float3, "pos" }, { ShaderDataType::Float2, "tex" } });

		m_SquareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[36];
		for (int i = 0; i < 36; i++) {
			squareIndices[i] = i;
		}

		Ref<IndexBuffer> squareIB = IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

		m_SquareVA->setIndexBuffer(squareIB);

		auto textureShader = m_ShaderLibrary.load("assets/shaders/OldTexture.glsl");

		m_Texture = Texture2D::create("assets/textures/Checkerboard.png");

		m_LogoTexture = Texture2D::create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<OpenGLShader>(textureShader)->bind();

		std::dynamic_pointer_cast<OpenGLShader>(textureShader)->uploadUniformInt("texture1", 0);
		std::dynamic_pointer_cast<OpenGLShader>(textureShader)->uploadUniformInt("texture2", 1);
	}

	void onUpdate(Timestep delta) override {

		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		RenderCommand::clear();

		m_CameraController.onUpdate(delta);

		Renderer::beginScene(m_CameraController.getCamera());
		auto textureShader = m_ShaderLibrary.get("OldTexture");

		m_Texture->bind(0);
		m_LogoTexture->bind(1);

		textureShader->bind();
		static float rotation = 0.f;
		rotation += delta * 30.f;

		for (int i = 0; i < 10; i++) {
			Renderer::submit(textureShader, m_SquareVA, rotateXYZ(Vector3(rotation, rotation, 0.f)) * translation(cubePositions[i]));
		}
		
		Renderer::endScene();
	}

  private:
	Vector3 cubePositions[10] = {
		Vector3( 0.0f,  0.0f,  0.0f), 
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
	Ref<Texture2D> m_Texture, m_LogoTexture;
	Ref<VertexArray> m_SquareVA;
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
