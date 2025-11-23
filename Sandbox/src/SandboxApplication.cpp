#include <Cabrankengine.h>

// --- Entry Point ---
#include "Cabrankengine/Core/EntryPoint.h"

#include <Cabrankengine/Events/KeyEvent.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <Cabrankengine/Renderer/Texture.h>
#include "Sandbox2D.h"
#include <Cabrankengine/Renderer/PerspectiveCamera.h>

using namespace cabrankengine;
using namespace cabrankengine::math;

class ExampleLayer : public cabrankengine::Layer {
  public:
	ExampleLayer() : Layer("Example"), m_Camera(-16.f / 9.f, 16.f / 9.f, -1.f, 1.f) {
		m_SquareVA = cabrankengine::VertexArray::create();

		float squareVertices[5 * 4] = {
			 0.5f,  0.5f,  0.0f,    1.0f, 1.0f,
			 0.5f, -0.5f,  0.0f,    1.0f, 0.0f,
			-0.5f, -0.5f,  0.0f,    0.0f, 0.0f,
			-0.5f,  0.5f,  0.0f,    0.0f, 1.0f
		};

		cabrankengine::Ref<cabrankengine::VertexBuffer> squareVB = cabrankengine::VertexBuffer::create(squareVertices, sizeof(squareVertices));

		squareVB->setLayout({ { cabrankengine::ShaderDataType::Float3, "pos" }, { cabrankengine::ShaderDataType::Float2, "tex" } });

		m_SquareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 3, 1, 2, 3 };

		cabrankengine::Ref<cabrankengine::IndexBuffer> squareIB = cabrankengine::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

		m_SquareVA->setIndexBuffer(squareIB);

		auto textureShader = m_ShaderLibrary.load("assets/shaders/OldTexture.glsl");

		m_Texture = cabrankengine::Texture2D::create("assets/textures/Checkerboard.png");

		m_LogoTexture = cabrankengine::Texture2D::create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<cabrankengine::OpenGLShader>(textureShader)->bind();

		std::dynamic_pointer_cast<cabrankengine::OpenGLShader>(textureShader)->uploadUniformInt("texture1", 0);
		std::dynamic_pointer_cast<cabrankengine::OpenGLShader>(textureShader)->uploadUniformInt("texture2", 1);
		
	}

	void onUpdate(cabrankengine::Timestep delta) override {

		cabrankengine::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		cabrankengine::RenderCommand::clear();

		Renderer::beginScene(m_Camera);
		auto textureShader = m_ShaderLibrary.get("OldTexture");

		m_Texture->bind(0);
		m_LogoTexture->bind(1);

		textureShader->bind();
		static float rotation = 0.f;
		rotation += delta * 90.f;
		cabrankengine::Renderer::submit(textureShader, m_SquareVA, rotateX(rotation));

		m_LogoTexture->bind();

		//cabrankengine::Renderer::submit(textureShader, m_SquareVA);
		Renderer::endScene();
	}

  private:
	OrthographicCamera m_Camera;
	cabrankengine::ShaderLibrary m_ShaderLibrary;
	cabrankengine::Ref<cabrankengine::Texture2D> m_Texture, m_LogoTexture;
	cabrankengine::Ref<cabrankengine::VertexArray> m_SquareVA;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f }; // Color of the square
};

class Sandbox : public cabrankengine::Application {
  public:
	Sandbox() {
		//pushLayer(new ExampleLayer());
		pushLayer(new Sandbox2D());
	}
	~Sandbox() {}
};

cabrankengine::Application* cabrankengine::createApplication() {
	return new Sandbox();
}
