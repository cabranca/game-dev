#include <Cabrankengine/Core/Application.h>
#include <Cabrankengine/Core/Window.h>
#include <Cabrankengine/Core/Logger.h>
#include <Cabrankengine/Events/ApplicationEvent.h>
#include <Cabrankengine/ImGui/ImGuiLayer.h>
#include <Cabrankengine/Renderer/Shader.h>
#include <Cabrankengine/Renderer/VertexArray.h>
#include <Cabrankengine/Renderer/Buffer.h>
#include <Cabrankengine/Renderer/Renderer.h>
#include <glad/glad.h> // TODO: check this inclusion. Due to the preprocessor definition, including glfw failed so I replaced it with glad.

namespace cabrankengine {

	Application* Application::s_Instance = nullptr;

	Application::Application() : m_Running(true)
	{
		CE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::create());
		m_Window->setEventCallback(CE_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));
		
		BufferLayout layout = {
			{ ShaderDataType::Float3, "pos" },
			{ ShaderDataType::Float4, "color" }
		};

		// TODO: this is not working on Linux on Debug and I do not know why.
		// It seems like there is a memory problem when calling setLayout but I couldn't trace it.
		vertexBuffer->setLayout(layout);

		m_VertexArray->addVertexBuffer(vertexBuffer);
		
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->setIndexBuffer(indexBuffer);

		m_SquareVA.reset(VertexArray::create());
		
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			 -0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::create(squareVertices, sizeof(squareVertices)));

		squareVB->setLayout({{ ShaderDataType::Float3, "pos" }});
		m_SquareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

		m_SquareVA->setIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 pos;
			layout(location = 1) in vec4 color;

			out vec3 v_pos;
			out vec4 v_color;

			void main()
			{
				v_pos = pos;
				v_color = color;
				gl_Position = vec4(pos, 1.0);
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

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		std::string blueVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 pos;

			out vec3 v_pos;

			void main()
			{
				v_pos = pos;
				gl_Position = vec4(pos, 1.0);
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
		m_BlueShader.reset(new Shader(blueVertexSrc, blueFragmentSrc));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running) {
			 RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
			 RenderCommand::clear();

			 Renderer::beginScene();

			 m_BlueShader->bind();
			 Renderer::submit(m_SquareVA);

			 m_Shader->bind();
			 Renderer::submit(m_VertexArray);

			 for (Layer* layer : m_LayerStack)
				 layer->onUpdate();

			 m_ImGuiLayer->begin();
			 for (Layer* layer : m_LayerStack)
				 layer->onImGuiRender();
			 m_ImGuiLayer->end();

			 m_Window->onUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(CE_BIND_EVENT_FN(Application::onWindowClose));
		CE_CORE_TRACE("{0}", e.toString());
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->onEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::pushLayer(Layer* layer) {
		m_LayerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* layer) {
		m_LayerStack.pushOverlay(layer);
		layer->onAttach();
	}

	void Application::popLayer(Layer* layer) {
		m_LayerStack.popLayer(layer);
	}

	void Application::popOverlay(Layer* layer) {
		m_LayerStack.popOverlay(layer);
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}


