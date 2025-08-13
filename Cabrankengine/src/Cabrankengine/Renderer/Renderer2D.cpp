#include <Cabrankengine/Renderer/Renderer2D.h>

#include <glm/gtc/matrix_transform.hpp>
#include <Cabrankengine/Core/Core.h>
#include <Cabrankengine/Renderer/Buffer.h>
#include <Cabrankengine/Renderer/RenderCommand.h>
#include <Cabrankengine/Renderer/Shader.h>
#include <Cabrankengine/Renderer/VertexArray.h>

using namespace glm;

namespace cabrankengine {

	struct Renderer2DStorage {
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
		Ref<Shader> TextureShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::init() {
		s_Data = new Renderer2DStorage();

		s_Data->QuadVertexArray = VertexArray::create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVB = VertexBuffer::create(squareVertices, sizeof(squareVertices));
		squareVB->setLayout({ { ShaderDataType::Float3, "pos" }, { ShaderDataType::Float2, "tex" } });
		s_Data->QuadVertexArray->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->setIndexBuffer(squareIB);

		s_Data->FlatColorShader = Shader::create("assets/shaders/FlatColor.glsl");
		s_Data->TextureShader = Shader::create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->bind();
		s_Data->TextureShader->setInt("u_Texture", 0);
	}

	void Renderer2D::shutdown() {
		delete s_Data;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera) {
		s_Data->FlatColorShader->bind();
		s_Data->FlatColorShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());

		s_Data->TextureShader->bind();
		s_Data->TextureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
	}

	void Renderer2D::endScene() {
		
	}

	void Renderer2D::drawQuad(const vec2& position, const vec2& size, const vec4& color) {
		drawQuad(vec3(position, 0.0f), size, color);
	}

	void Renderer2D::drawQuad(const vec3& position, const vec2& size, const vec4& color) {
		s_Data->FlatColorShader->bind();
		s_Data->FlatColorShader->setFloat4("u_Color", color);

		mat4 transform = translate(mat4(1.f), position) * scale(mat4(1.f), vec3(size, 1.0f));
		s_Data->FlatColorShader->setMat4("u_Transform", transform);
		
		s_Data->QuadVertexArray->bind();
		RenderCommand::drawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::drawQuad(const vec2& position, const vec2& size, const Ref<Texture2D>& texture) {
		drawQuad(vec3(position, 0.0f), size, texture);
	}

	void Renderer2D::drawQuad(const vec3& position, const vec2& size, const Ref<Texture2D>& texture) {
		s_Data->TextureShader->bind();

		mat4 transform = translate(mat4(1.f), position) * scale(mat4(1.f), vec3(size, 1.0f));
		s_Data->TextureShader->setMat4("u_Transform", transform);

		texture->bind();
		s_Data->QuadVertexArray->bind();
		RenderCommand::drawIndexed(s_Data->QuadVertexArray);
	}
}
