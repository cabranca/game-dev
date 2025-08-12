#include <Cabrankengine.h>

#include <Cabrankengine/Events/KeyEvent.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <Cabrankengine/Renderer/Texture.h>

class ExampleLayer : public cabrankengine::Layer {
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.f), m_SquarePosition(0.f) {
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
		if (cabrankengine::Input::isKeyPressed(cabrankengine::Key::Left))
			m_CameraPosition.x -= m_CameraSpeed * delta;
		else if (cabrankengine::Input::isKeyPressed(cabrankengine::Key::Right))
			m_CameraPosition.x += m_CameraSpeed * delta;
		if (cabrankengine::Input::isKeyPressed(cabrankengine::Key::Down))
			m_CameraPosition.y -= m_CameraSpeed * delta;
		else if (cabrankengine::Input::isKeyPressed(cabrankengine::Key::Up))
			m_CameraPosition.y += m_CameraSpeed * delta;

		if (cabrankengine::Input::isKeyPressed(cabrankengine::Key::J))
			m_SquarePosition.x -= m_SquareSpeed * delta;
		else if (cabrankengine::Input::isKeyPressed(cabrankengine::Key::L))
			m_SquarePosition.x += m_SquareSpeed * delta;
		if (cabrankengine::Input::isKeyPressed(cabrankengine::Key::K))
			m_SquarePosition.y -= m_SquareSpeed * delta;
		else if (cabrankengine::Input::isKeyPressed(cabrankengine::Key::I))
			m_SquarePosition.y += m_SquareSpeed * delta;

		else if (cabrankengine::Input::isKeyPressed(cabrankengine::Key::R))
			m_CameraRotation += m_CameraRotationSpeed * delta;

		cabrankengine::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		cabrankengine::RenderCommand::clear();

		m_Camera.setPosition(m_CameraPosition);
		m_Camera.setRotation(m_CameraRotation);

		cabrankengine::Renderer::beginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(0.1f));

		auto textureShader = m_ShaderLibrary.get("Texture");

		m_Texture->bind();
		cabrankengine::Renderer::submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.f), glm::vec3(1.5f)));

		m_LogoTexture->bind();
		cabrankengine::Renderer::submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.f), glm::vec3(1.5f)));

		//cabrankengine::Renderer::submit(m_Shader, m_VertexArray);
		cabrankengine::Renderer::endScene();
	}

	void onImGuiRender() override {
		ImGui::Begin("Settings");

		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::End();
	}
	
	void onEvent(cabrankengine::Event& event) override {
		cabrankengine::EventDispatcher dispatcher(event);
		dispatcher.dispatch<cabrankengine::KeyPressedEvent>(bind_event_fn(&ExampleLayer::onKeyPressedEvent, this));
	}

	bool onKeyPressedEvent(cabrankengine::KeyPressedEvent& event) {
		

		return false;
	}

private:
	cabrankengine::ShaderLibrary m_ShaderLibrary;
	cabrankengine::Ref<cabrankengine::VertexArray> m_SquareVA;

	cabrankengine::Ref<cabrankengine::Texture2D> m_Texture, m_LogoTexture;

	cabrankengine::OrthographicCamera m_Camera; // Camera for the scene
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 1.f; // Speed of the camera movement
	float m_CameraRotation = 0.f; // Rotation of the camera
	float m_CameraRotationSpeed = 1.f; // Speed of the camera rotation

	glm::vec3 m_SquarePosition;
	float m_SquareSpeed = 1.f; // Speed of the square

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
