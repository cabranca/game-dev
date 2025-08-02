#include <Platform/OpenGL/OpenGLBuffer.h>

#include <glad/glad.h>

namespace cabrankengine {

	// --------------------- VERTEX BUFFER --------------------- //

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) : m_RendererId() {
		glCreateBuffers(1, &m_RendererId); // TODO: research this functions. Cherno thought that "create" elminiated the need of "bind" but it does not.
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		glDeleteBuffers(1, &m_RendererId);
	}

	void OpenGLVertexBuffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	}

	void OpenGLVertexBuffer::unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// --------------------- INDEX BUFFER --------------------- //

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : m_RendererId(), m_Count(count) {
		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		glDeleteBuffers(1, &m_RendererId);
	}

	void OpenGLIndexBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
	}

	void OpenGLIndexBuffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}