#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace cabrankengine::platform::opengl {

	using namespace rendering;

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec) : m_Specs(spec) {
		invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer() {
		glDeleteFramebuffers(1, &m_RendererID);
	}

	void OpenGLFrameBuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    }

	void OpenGLFrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

	const rendering::FrameBufferSpecification& OpenGLFrameBuffer::getSpecification() const {
		return m_Specs;
	}

	uint32_t OpenGLFrameBuffer::getColorAttachmentRendererID() const {
		return m_ColorAttachment;
	}

	void OpenGLFrameBuffer::invalidate() {
		if (m_RendererID) {
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_ColorAttachment);
			glDeleteTextures(1, &m_DepthAttachment);
		}

		glCreateFramebuffers(1, &m_RendererID);
		
		// Color attachment
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		glTextureStorage2D(m_ColorAttachment, 1, GL_RGBA8, m_Specs.width, m_Specs.height);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glNamedFramebufferTexture(m_RendererID, GL_COLOR_ATTACHMENT0, m_ColorAttachment, 0);
		
		// Color attachment
		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glTextureStorage2D(m_DepthAttachment, 1, GL_DEPTH24_STENCIL8, m_Specs.width, m_Specs.height);
		glNamedFramebufferTexture(m_RendererID, GL_DEPTH_STENCIL_ATTACHMENT, m_DepthAttachment, 0);

		auto status = glCheckNamedFramebufferStatus(m_RendererID, GL_FRAMEBUFFER);
		CE_CORE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer is incomplete!");
	}
} // namespace cabrankengine::platform::opengl
