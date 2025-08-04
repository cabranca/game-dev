#include <Cabrankengine.h>
#include <Cabrankengine/Events/KeyEvent.h>

class ExampleLayer : public cabrankengine::Layer {
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.f) {
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
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			 -0.75f,  0.75f, 0.0f
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

			out vec3 v_pos;
			out vec4 v_color;

			void main()
			{
				v_pos = pos;
				v_color = color;
				gl_Position = u_viewProjection * vec4(pos, 1.0);
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

		m_Shader.reset(new cabrankengine::Shader(vertexSrc, fragmentSrc));

		std::string blueVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 pos;

			uniform mat4 u_viewProjection;

			out vec3 v_pos;

			void main()
			{
				v_pos = pos;
				gl_Position = u_viewProjection * vec4(pos, 1.0);
			}
		)";

		std::string blueFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_pos;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";
		m_BlueShader.reset(new cabrankengine::Shader(blueVertexSrc, blueFragmentSrc));
	}
	
	void onUpdate(cabrankengine::Timestep delta) override {
		CE_TRACE("Delta time: {0}s ({1}ms)", delta.getSeconds(), delta.getMilliseconds());

		if (cabrankengine::Input::isKeyPressed(cabrankengine::Key::Left))
			m_CameraPosition.x -= m_CameraSpeed * delta;
		else if (cabrankengine::Input::isKeyPressed(cabrankengine::Key::Right))
			m_CameraPosition.x += m_CameraSpeed * delta;
		if (cabrankengine::Input::isKeyPressed(cabrankengine::Key::Down))
			m_CameraPosition.y -= m_CameraSpeed * delta;
		else if (cabrankengine::Input::isKeyPressed(cabrankengine::Key::Up))
			m_CameraPosition.y += m_CameraSpeed * delta;

		else if (cabrankengine::Input::isKeyPressed(cabrankengine::Key::R))
			m_CameraRotation += m_CameraRotationSpeed * delta;

		cabrankengine::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		cabrankengine::RenderCommand::clear();

		m_Camera.setPosition(m_CameraPosition);
		m_Camera.setRotation(m_CameraRotation);

		cabrankengine::Renderer::beginScene(m_Camera);

		cabrankengine::Renderer::submit(m_BlueShader, m_SquareVA);
		cabrankengine::Renderer::submit(m_Shader, m_VertexArray);
	}

	void onImGuiRender() override {

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

	std::shared_ptr<cabrankengine::Shader> m_BlueShader;
	std::shared_ptr<cabrankengine::VertexArray> m_SquareVA;

	cabrankengine::OrthographicCamera m_Camera; // Camera for the scene
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 1.f; // Speed of the camera movement
	float m_CameraRotation = 0.f; // Rotation of the camera
	float m_CameraRotationSpeed = 1.f; // Speed of the camera rotation
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
