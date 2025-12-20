#include <pch.h>
#include "Renderer.h"

#include "Lights.h"
#include "Material.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include "StorageBuffer.h"
#include "TextRenderer.h"
#include "VertexArray.h"


namespace cabrankengine::rendering {

	using namespace math;

	// GLSL std430 alignment rules:
	// vec4 = 16 bytes. scalar = 4 bytes.
	// Structs align to largest member (16 bytes).
	struct PointLightGPU {
		float position[4];  // x, y, z, padding
		float radiance[4];   // r, g, b, padding (o intensidad)
		float constant;
		float linear;
		float quadratic;
		float padding;      // Para completar 48 bytes (múltiplo de 16)
	};

	struct LightBufferHeader {
		int count;
		int padding[3];     // Alinear a 16 bytes antes del array
	};

	void Renderer::init() {
		CE_PROFILE_FUNCTION();

		RenderCommand::init();
		Renderer2D::init();
		TextRenderer::init();
		s_SceneData->lightSSBO = StorageBuffer::create(sizeof(LightBufferHeader) + sizeof(PointLightGPU));
	}

	void Renderer::shutdown() {
		Renderer2D::shutdown();
	}

	void Renderer::beginScene(const math::Mat4& projection, const math::Mat4& view, const LightEnvironment& environment) {
		s_SceneData->projectionMatrix = projection;
		s_SceneData->viewMatrix = view;
		s_SceneData->lightEnvironment = environment;

		uploadLightEnvironment();
	}

	void Renderer::endScene() {
		
	}

	void Renderer::submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const Mat4& transform) {
		shader->bind();
		shader->setMat4("projection", s_SceneData->projectionMatrix);
		shader->setMat4("view", s_SceneData->viewMatrix);
		shader->setMat4("model", transform);
		RenderCommand::drawIndexed(vertexArray);
	}

	void Renderer::submit(const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const Mat4& transform) {
		material->bind();
		auto shader = material->getShader();
		shader->setMat4("projection", s_SceneData->projectionMatrix);
		shader->setMat4("view", s_SceneData->viewMatrix);
		shader->setMat4("model", transform);
		shader->setFloat3("dirLight.direction", s_SceneData->lightEnvironment.DirLight.direction);
		shader->setFloat3("dirLight.radiance", s_SceneData->lightEnvironment.DirLight.radiance);
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}

	void Renderer::onWindowResize(uint32_t width, uint32_t height) {
		RenderCommand::setViewport(0, 0, width, height);
	}
	
	void Renderer::uploadLightEnvironment() {
		size_t count = s_SceneData->lightEnvironment.PointLights.size();
		LightBufferHeader header;
		header.count = static_cast<int>(count);

		std::vector<uint8_t> bufferData;
		bufferData.resize(sizeof(LightBufferHeader) + count * sizeof(PointLightGPU));

		memcpy(bufferData.data(), &header, sizeof(LightBufferHeader));

		PointLightGPU* currentLight = reinterpret_cast<PointLightGPU*>(bufferData.data() + sizeof(LightBufferHeader));

		for (const auto& light : s_SceneData->lightEnvironment.PointLights) {
			currentLight->position[0] = light.position.x;
			currentLight->position[1] = light.position.y;
			currentLight->position[2] = light.position.z;
			currentLight->position[3] = 1.f;
			
			currentLight->radiance[0] = light.radiance.x;
			currentLight->radiance[1] = light.radiance.y;
			currentLight->radiance[2] = light.radiance.z;
			currentLight->radiance[3] = 0.f;

			currentLight->constant = light.constant;
			currentLight->linear = light.linear;
			currentLight->quadratic = light.quadratic;

			currentLight++;
		}

		s_SceneData->lightSSBO->setData(bufferData.data(), bufferData.size());
		s_SceneData->lightSSBO->bind(0);
	}
} // namespace cabrankengine::rendering
