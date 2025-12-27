#include <pch.h>
#include "MetalRendererAPI.h"

#include <Cabrankengine/Renderer/VertexArray.h>
#include <Cabrankengine/Renderer/Buffer.h>

namespace cabrankengine::platform::metal {

	using namespace rendering;

	void MetalRendererAPI::init() {
		CE_PROFILE_FUNCTION();

	}

	void MetalRendererAPI::setClearColor(const math::Vector4& color) {
	}

	void MetalRendererAPI::clear() {
	}

	void MetalRendererAPI::draw(const Ref<VertexArray>& vertexArray) {
	}

	void MetalRendererAPI::drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) {
	}

	void MetalRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
	}
}
