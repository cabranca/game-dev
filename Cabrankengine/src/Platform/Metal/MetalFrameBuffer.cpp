#include <pch.h>
#include "MetalFrameBuffer.h"

namespace cabrankengine::platform::metal {

	using namespace rendering;

	MetalFrameBuffer::MetalFrameBuffer(const FrameBufferSpecification& spec) : m_Specs(spec) {
		invalidate();
	}

	MetalFrameBuffer::~MetalFrameBuffer() {
	}

	void MetalFrameBuffer::bind() {
    }

	void MetalFrameBuffer::unbind() {
    }

	const rendering::FrameBufferSpecification& MetalFrameBuffer::getSpecification() const {
		return m_Specs;
	}

	uint32_t MetalFrameBuffer::getColorAttachmentRendererID() const {
		return m_ColorAttachment;
	}

	void MetalFrameBuffer::invalidate() {
		if (m_RendererID) {
		}

	}
} // namespace cabrankengine::platform::metal
