#include <Cabrankengine/Core/Application.h>
#include <Cabrankengine/Core/Window.h>
#include <Cabrankengine/Core/Logger.h>
#include <Cabrankengine/Events/ApplicationEvent.h>
#include <Cabrankengine/ImGui/ImGuiLayer.h>
#include <Cabrankengine/Renderer/Shader.h>
#include <Cabrankengine/Renderer/Buffer.h>
#include <glad/glad.h> // TODO: check this inclusion. Due to the preprocessor definition, including glfw failed so I replaced it with glad.

namespace cabrankengine {

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataType2OpenGLBaseType(ShaderDataType type) {
		switch(type) {
			case ShaderDataType::Float:   return GL_FLOAT;
			case ShaderDataType::Float2:  return GL_FLOAT;
			case ShaderDataType::Float3:  return GL_FLOAT;
			case ShaderDataType::Float4:  return GL_FLOAT;
			case ShaderDataType::Mat3:    return GL_FLOAT;
			case ShaderDataType::Mat4:    return GL_FLOAT;
			case ShaderDataType::Int:     return GL_INT;
			case ShaderDataType::Int2:    return GL_INT;
			case ShaderDataType::Int3:    return GL_INT;
			case ShaderDataType::Int4:    return GL_INT;
			case ShaderDataType::Bool:    return GL_BOOL;
		}

		CE_CORE_ASSERT(false, "Unknown Shader Type!");
		return 0;
	}

	Application::Application() : m_Running(true)
	{
		CE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::create());
		m_Window->setEventCallback(CE_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		m_VertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));
		
		{
			BufferLayout layout = {
			{ ShaderDataType::Float3, "pos" },
			{ ShaderDataType::Float4, "color" }
			};

			// TODO: this is not working on Linux on Debug and I do not know why.
			// It seems like there is a memory problem when calling setLayout but I couldn't trace it.
			m_VertexBuffer->setLayout(layout);
		}

		const auto& layout = m_VertexBuffer->getLayout();
		uint32_t index = 0;
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, 
				element.getComponentCount(), 
				ShaderDataType2OpenGLBaseType(element.Type), 
				element.Normalized? GL_TRUE : GL_FALSE, 
				layout.getStride(), 
				(const void*)element.Offset);
			index++;
		}

		
		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

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
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running) {
			 glClearColor(0.1f, 0.1f, 0.1f, 1.f);
			 glClear(GL_COLOR_BUFFER_BIT);

			 m_Shader->bind();
			 glBindVertexArray(m_VertexArray);
			 glDrawElements(GL_TRIANGLES, m_IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

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


