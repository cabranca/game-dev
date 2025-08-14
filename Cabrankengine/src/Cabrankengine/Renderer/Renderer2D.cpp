#include <Cabrankengine/Renderer/Renderer2D.h>

#include <glm/gtc/matrix_transform.hpp>
#include <Cabrankengine/Core/Core.h>
#include <Cabrankengine/Debug/Instrumentator.h>
#include <Cabrankengine/Renderer/Buffer.h>
#include <Cabrankengine/Renderer/RenderCommand.h>
#include <Cabrankengine/Renderer/Shader.h>
#include <Cabrankengine/Renderer/VertexArray.h>

using namespace glm;

namespace cabrankengine {

	struct Renderer2DStorage {
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::init() {
		CE_PROFILE_FUNCTION();

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

		s_Data->WhiteTexture = Texture2D::create(TextureSpecification());
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

		s_Data->TextureShader = Shader::create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->bind();
		s_Data->TextureShader->setInt("u_Texture", 0);
	}

	void Renderer2D::shutdown() {
		CE_PROFILE_FUNCTION();

		delete s_Data;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera) {
		CE_PROFILE_FUNCTION();

		s_Data->TextureShader->bind();
		s_Data->TextureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
	}

	void Renderer2D::endScene() {
		CE_PROFILE_FUNCTION();
	}

	void Renderer2D::drawQuad(const vec2& position, const vec2& size, const vec4& color, float tilingFactor) {
		drawQuad(vec3(position, 0.0f), size, color, tilingFactor);
	}

	void Renderer2D::drawQuad(const vec3& position, const vec2& size, const vec4& color, float tilingFactor) {
		CE_PROFILE_FUNCTION();

		s_Data->TextureShader->setFloat4("u_Color", color);
		s_Data->TextureShader->setFloat("u_TilingFactor", tilingFactor);
		s_Data->WhiteTexture->bind();

		mat4 transform = translate(mat4(1.f), position) * scale(mat4(1.f), vec3(size, 1.0f));
		s_Data->TextureShader->setMat4("u_Transform", transform);
		
		s_Data->QuadVertexArray->bind();
		RenderCommand::drawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::drawQuad(const vec2& position, const vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor) {
		drawQuad(vec3(position, 0.0f), size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawQuad(const vec3& position, const vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor) {
		CE_PROFILE_FUNCTION();

		mat4 transform = translate(mat4(1.f), position) * scale(mat4(1.f), vec3(size, 1.0f));
		s_Data->TextureShader->setMat4("u_Transform", transform);
		s_Data->TextureShader->setFloat4("u_Color", tintColor);
		s_Data->TextureShader->setFloat("u_TilingFactor", tilingFactor);
		
		texture->bind();
		s_Data->QuadVertexArray->bind();
		RenderCommand::drawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::drawRotatedQuad(const vec2& position, const vec2& size, float rotation, const vec4& color, float tilingFactor) {
		drawRotatedQuad(vec3(position, 0.0f), size, rotation, color, tilingFactor);
	}

	void Renderer2D::drawRotatedQuad(const vec3& position, const vec2& size, float rotation, const vec4& color, float tilingFactor) {
		CE_PROFILE_FUNCTION();
		s_Data->TextureShader->setFloat4("u_Color", color);
		s_Data->TextureShader->setFloat("u_TilingFactor", tilingFactor);
		s_Data->WhiteTexture->bind();
		mat4 transform = translate(mat4(1.f), position) * rotate(mat4(1.f), radians(rotation), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.f), vec3(size, 1.0f));
		s_Data->TextureShader->setMat4("u_Transform", transform);
		s_Data->QuadVertexArray->bind();
		RenderCommand::drawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::drawRotatedQuad(const vec2& position, const vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor) {
		drawRotatedQuad(vec3(position, 0.0f), size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawRotatedQuad(const vec3& position, const vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor) {
		CE_PROFILE_FUNCTION();
		mat4 transform = translate(mat4(1.f), position) * rotate(mat4(1.f), radians(rotation), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.f), vec3(size, 1.0f));
		s_Data->TextureShader->setMat4("u_Transform", transform);
		s_Data->TextureShader->setFloat4("u_Color", tintColor);
		s_Data->TextureShader->setFloat("u_TilingFactor", tilingFactor);
		
		texture->bind();
		s_Data->QuadVertexArray->bind();
		RenderCommand::drawIndexed(s_Data->QuadVertexArray);
	}
}
