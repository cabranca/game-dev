#include <Platform/OpenGL/OpenGLBuffer.h>

#include <glad/glad.h>
#include <Cabrankengine/Debug/Instrumentator.h>

namespace cabrankengine {

	// --------------------- VERTEX BUFFER --------------------- //

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) : m_RendererId() {
		CE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererId); // TODO: research this functions. Cherno thought that "create" elminiated the need of "bind" but it does not.
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		CE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererId);
	}

	void OpenGLVertexBuffer::bind() const {
		CE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	}

	void OpenGLVertexBuffer::unbind() const {
		CE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// --------------------- INDEX BUFFER --------------------- //

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : m_RendererId(), m_Count(count) {
		CE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		CE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererId);
	}

	void OpenGLIndexBuffer::bind() const {
		CE_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
	}

	void OpenGLIndexBuffer::unbind() const {
		CE_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}