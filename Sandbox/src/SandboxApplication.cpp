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
	ExampleLayer() : Layer("Example"), m_Camera(-16.f / 9.f, 16.f / 9.f, -1.f, 1.f) {
		m_SquareVA = VertexArray::create();

		float squareVertices[5 * 4] = {
			 0.5f,  0.5f,  0.0f,    1.0f, 1.0f,
			 0.5f, -0.5f,  0.0f,    1.0f, 0.0f,
			-0.5f, -0.5f,  0.0f,    0.0f, 0.0f,
			-0.5f,  0.5f,  0.0f,    0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVB = VertexBuffer::create(squareVertices, sizeof(squareVertices));

		squareVB->setLayout({ { ShaderDataType::Float3, "pos" }, { ShaderDataType::Float2, "tex" } });

		m_SquareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 3, 1, 2, 3 };

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

		Renderer::beginScene(m_Camera);
		auto textureShader = m_ShaderLibrary.get("OldTexture");

		m_Texture->bind(0);
		m_LogoTexture->bind(1);

		textureShader->bind();
		static float rotation = 0.f;
		rotation += delta * 90.f;
		Renderer::submit(textureShader, m_SquareVA, rotateX(rotation));

		m_LogoTexture->bind();

		//Renderer::submit(textureShader, m_SquareVA);
		Renderer::endScene();
	}

  private:
	OrthographicCamera m_Camera;
	ShaderLibrary m_ShaderLibrary;
	Ref<Texture2D> m_Texture, m_LogoTexture;
	Ref<VertexArray> m_SquareVA;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f }; // Color of the square
};

class Sandbox : public Application {
  public:
	Sandbox() {
		//pushLayer(new ExampleLayer());
		pushLayer(new Sandbox2D());
	}
	~Sandbox() {}
};

Application* cabrankengine::createApplication() {
	return new Sandbox();
}
