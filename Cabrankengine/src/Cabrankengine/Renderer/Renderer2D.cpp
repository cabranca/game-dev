#include <Cabrankengine/Renderer/Renderer2D.h>

#include <Cabrankengine/Core/Core.h>
#include <Cabrankengine/Renderer/Buffer.h>
#include <Cabrankengine/Renderer/RenderCommand.h>
#include <Cabrankengine/Renderer/Shader.h>
#include <Cabrankengine/Renderer/VertexArray.h>
#include <Platform/OpenGL/OpenGLShader.h>

namespace cabrankengine {

	struct Renderer2DStorage {
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::init() {
		s_Data = new Renderer2DStorage();

		s_Data->QuadVertexArray = VertexArray::create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		Ref<VertexBuffer> squareVB = VertexBuffer::create(squareVertices, sizeof(squareVertices));
		squareVB->setLayout({ { ShaderDataType::Float3, "a_Position" } });
		s_Data->QuadVertexArray->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->setIndexBuffer(squareIB);

		s_Data->FlatColorShader = Shader::create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::shutdown() {
		delete s_Data;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera) {
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->uploadUniformMat4("u_ViewProjection", camera.getViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->uploadUniformMat4("u_Transform", glm::mat4(1.f));
	}

	void Renderer2D::endScene() {
		
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		drawQuad(glm::vec3(position, 0.0f), size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->uploadUniformFloat4("u_Color", color);
		s_Data->QuadVertexArray->bind();
		RenderCommand::drawIndexed(s_Data->QuadVertexArray);
	}
}
