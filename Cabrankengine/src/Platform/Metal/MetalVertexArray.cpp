#include <pch.h>
#include "MetalVertexArray.h"

#include <Cabrankengine/Renderer/Buffer.h>

namespace cabrankengine::platform::metal {

	using namespace rendering;


	MetalVertexArray::MetalVertexArray() : m_RendererId() {
		CE_PROFILE_FUNCTION();

	}

	MetalVertexArray::~MetalVertexArray() {
		CE_PROFILE_FUNCTION();

	}

	void MetalVertexArray::bind() const {
		CE_PROFILE_FUNCTION();

	}

	void MetalVertexArray::unbind() const {
		CE_PROFILE_FUNCTION();

	}

	void MetalVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
		CE_PROFILE_FUNCTION();

		CE_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex Buffer has no layout!");


		m_VertexBuffers.push_back(vertexBuffer);
	}

	void MetalVertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
		CE_PROFILE_FUNCTION();

		indexBuffer->bind();

		m_IndexBuffer = indexBuffer;
	}
}
