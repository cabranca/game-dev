#include <Cabrankengine.h>

#include <Cabrankengine/Events/KeyEvent.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

class ExampleLayer : public cabrankengine::Layer {
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.f), m_SquarePosition(0.f) {
		m_VertexArray.reset(cabrankengine::VertexArray::create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<cabrankengine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(cabrankengine::VertexBuffer::create(vertices, sizeof(vertices)));

		cabrankengine::BufferLayout layout = {
			{ cabrankengine::ShaderDataType::Float3, "pos" },
			{ cabrankengine::ShaderDataType::Float4, "color" }
		};

		// TODO: this is not working on Linux on Release and I do not know why.
		// It seems like there is a memory problem when calling setLayout but I couldn't trace it.
		vertexBuffer->setLayout(layout);

		m_VertexArray->addVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<cabrankengine::IndexBuffer> indexBuffer;
		indexBuffer.reset(cabrankengine::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->setIndexBuffer(indexBuffer);

		m_SquareVA.reset(cabrankengine::VertexArray::create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<cabrankengine::VertexBuffer> squareVB;
		squareVB.reset(cabrankengine::VertexBuffer::create(squareVertices, sizeof(squareVertices)));

		squareVB->setLayout({ { cabrankengine::ShaderDataType::Float3, "pos" } });
		m_SquareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<cabrankengine::IndexBuffer> squareIB;
		squareIB.reset(cabrankengine::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

		m_SquareVA->setIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 pos;
			layout(location = 1) in vec4 color;

			uniform mat4 u_viewProjection;
			uniform mat4 u_Transform;

			out vec3 v_pos;
			out vec4 v_color;

			void main()
			{
				v_pos = pos;
				v_color = color;
				gl_Position = u_viewProjection * u_Transform * vec4(pos, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_pos;
			in vec4 v_color;

			void main()
			{
				color = vec4(v_pos + 0.5, 1.0);
				color = v_color;
			}
		)";

		m_Shader.reset(cabrankengine::Shader::create(vertexSrc, fragmentSrc));

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 pos;

			uniform mat4 u_viewProjection;
			uniform mat4 u_Transform;

			out vec3 v_pos;

			void main()
			{
				v_pos = pos;
				gl_Position = u_viewProjection * u_Transform * vec4(pos, 1.0);
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_pos;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";
		m_FlatColorShader.reset(cabrankengine::Shader::create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));
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

		std::dynamic_pointer_cast<cabrankengine::OpenGLShader>(m_FlatColorShader)->bind();
		std::dynamic_pointer_cast<cabrankengine::OpenGLShader>(m_FlatColorShader)->uploadUniformFloat3("u_Color", m_SquareColor);

		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				glm::vec3 pos(i * 0.11f, j * 0.11f, 0.f);
				glm::mat4 squareTransform = glm::translate(glm::mat4(1.f), pos) * scale;
				cabrankengine::Renderer::submit(m_FlatColorShader, m_SquareVA, squareTransform);
			}
		}
		cabrankengine::Renderer::submit(m_Shader, m_VertexArray);
		cabrankengine::Renderer::endScene();
	}

	void onImGuiRender() override {
		ImGui::Begin("Settings");

		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::End();
	}
	
	void onEvent(cabrankengine::Event& event) override {
		cabrankengine::EventDispatcher dispatcher(event);
		dispatcher.dispatch<cabrankengine::KeyPressedEvent>(CE_BIND_EVENT_FN(ExampleLayer::onKeyPressedEvent));
	}

	bool onKeyPressedEvent(cabrankengine::KeyPressedEvent& event) {
		

		return false;
	}

private:
	std::shared_ptr<cabrankengine::Shader> m_Shader;
	std::shared_ptr<cabrankengine::VertexArray> m_VertexArray;

	std::shared_ptr<cabrankengine::Shader> m_FlatColorShader;
	std::shared_ptr<cabrankengine::VertexArray> m_SquareVA;

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

cabrankengine::Application* cabrankengine::CreateApplication() {
	return new Sandbox();
}
