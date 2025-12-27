#include <pch.h>
#include "MetalBuffer.h"

namespace cabrankengine::platform::metal {

	// --------------------- VERTEX BUFFER --------------------- //

	MetalVertexBuffer::MetalVertexBuffer(uint32_t size) : m_RendererId() {
		CE_PROFILE_FUNCTION();

	}

	MetalVertexBuffer::MetalVertexBuffer(void* vertices, uint32_t size) : m_RendererId() {
		CE_PROFILE_FUNCTION();

	}

	MetalVertexBuffer::~MetalVertexBuffer() {
		CE_PROFILE_FUNCTION();

	}

	void MetalVertexBuffer::bind() const {
		CE_PROFILE_FUNCTION();

	}

	void MetalVertexBuffer::unbind() const {
		CE_PROFILE_FUNCTION();

	}

	void MetalVertexBuffer::setData(const void* data, uint32_t size) {
		CE_PROFILE_FUNCTION();
	}

	// --------------------- INDEX BUFFER --------------------- //

	MetalIndexBuffer::MetalIndexBuffer(uint32_t* indices, uint32_t count) : m_RendererId(), m_Count(count) {
		CE_PROFILE_FUNCTION();

	}

	MetalIndexBuffer::~MetalIndexBuffer() {
		CE_PROFILE_FUNCTION();

	}

	void MetalIndexBuffer::bind() const {
		CE_PROFILE_FUNCTION();

	}

	void MetalIndexBuffer::unbind() const {
		CE_PROFILE_FUNCTION();

	}
}
