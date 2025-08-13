#include <Cabrankengine.h>

#include <Cabrankengine/Events/KeyEvent.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <Cabrankengine/Renderer/Texture.h>

class ExampleLayer : public cabrankengine::Layer {
public:
	ExampleLayer() : Layer("Example"), m_CameraController(1600.f / 900.f, true) {
		m_SquareVA = cabrankengine::VertexArray::create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		cabrankengine::Ref<cabrankengine::VertexBuffer> squareVB;
		squareVB = cabrankengine::VertexBuffer::create(squareVertices, sizeof(squareVertices));

		squareVB->setLayout({ { cabrankengine::ShaderDataType::Float3, "pos" }, {cabrankengine::ShaderDataType::Float2, "tex"} });
		m_SquareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		cabrankengine::Ref<cabrankengine::IndexBuffer> squareIB;
		squareIB = cabrankengine::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

		m_SquareVA->setIndexBuffer(squareIB);

		auto textureShader = m_ShaderLibrary.load("assets/shaders/Texture.glsl");

		m_Texture = cabrankengine::Texture2D::create("assets/textures/Checkerboard.png");
		m_LogoTexture = cabrankengine::Texture2D::create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<cabrankengine::OpenGLShader>(textureShader)->bind();
		std::dynamic_pointer_cast<cabrankengine::OpenGLShader>(textureShader)->uploadUniformInt("u_Texture", 0);
	}
	
	void onUpdate(cabrankengine::Timestep delta) override {
		m_CameraController.onUpdate(delta);

		cabrankengine::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		cabrankengine::RenderCommand::clear();

		cabrankengine::Renderer::beginScene(m_CameraController.getCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(0.1f));

		auto textureShader = m_ShaderLibrary.get("Texture");

		m_Texture->bind();
		cabrankengine::Renderer::submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.f), glm::vec3(1.5f)));

		m_LogoTexture->bind();
		cabrankengine::Renderer::submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.f), glm::vec3(1.5f)));

		cabrankengine::Renderer::endScene();
	}

	void onImGuiRender() override {
		ImGui::Begin("Settings");

		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::End();
	}
	
	void onEvent(cabrankengine::Event& event) override {
		m_CameraController.onEvent(event);
	}

private:
	cabrankengine::ShaderLibrary m_ShaderLibrary;
	cabrankengine::Ref<cabrankengine::VertexArray> m_SquareVA;

	cabrankengine::Ref<cabrankengine::Texture2D> m_Texture, m_LogoTexture;

	cabrankengine::OrthographicCameraController m_CameraController; // Camera controller for the scene
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f }; // Color of the square
};

class Sandbox : public cabrankengine::Application {
public:
	Sandbox() {
		pushLayer(new ExampleLayer());
	}
	~Sandbox() {

	}
};

cabrankengine::Application* cabrankengine::createApplication() {
	return new Sandbox();
}
