#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <Platform/OpenGL/OpenGLShader.h>

using namespace cabrankengine;

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::onAttach() {
	m_SquareVA = VertexArray::create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Ref<VertexBuffer> squareVB;
	squareVB = VertexBuffer::create(squareVertices, sizeof(squareVertices));
	squareVB->setLayout({
		{ ShaderDataType::Float3, "a_Position" }
		});
	m_SquareVA->addVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Ref<IndexBuffer> squareIB;
	squareIB = IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVA->setIndexBuffer(squareIB);

	m_FlatColorShader = Shader::create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::onDetach() {
	// Cleanup code here
}

void Sandbox2D::onUpdate(cabrankengine::Timestep delta) {
	// Update
	m_CameraController.onUpdate(delta);

	// Render
	RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::clear();

	Renderer::beginScene(m_CameraController.getCamera());

	std::dynamic_pointer_cast<OpenGLShader>(m_FlatColorShader)->bind();
	std::dynamic_pointer_cast<OpenGLShader>(m_FlatColorShader)->uploadUniformFloat4("u_Color", m_SquareColor);

	Renderer::submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Renderer::endScene();
}

void Sandbox2D::onImGuiRender() {
	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void Sandbox2D::onEvent(cabrankengine::Event& e) {
	m_CameraController.onEvent(e);
}
