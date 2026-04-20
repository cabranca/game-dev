#pragma once

#include <Cabrankengine/Renderer/Buffer.h>

namespace cbk::platform::opengl {

	class OpenGLVertexBuffer : public rendering::VertexBuffer {
		public:
			OpenGLVertexBuffer(uint32_t size);
			OpenGLVertexBuffer(void* vertices, uint32_t size);
			~OpenGLVertexBuffer();

			// Binds the vertex buffer so that it can be used in rendering.
			virtual void bind() const override;

			// Unbinds the vertex buffer so that it is no longer used in rendering.
			virtual void unbind() const override;

			// Returns the structure of the vertex buffer, which is defined by the BufferLayout.
			virtual const rendering::BufferLayout& getLayout() const override { return m_Layout; }
		
			// Sets the layout of the vertex buffer, which defines how the vertex attributes are organized.
			virtual void setLayout(const rendering::BufferLayout& layout) override { m_Layout = layout; }

			virtual void setData(const void* data, uint32_t size) override;

		private:
			uint32_t m_RendererId; // OpenGL renderer ID for the vertex buffer
			rendering::BufferLayout m_Layout; // Layout of the vertex buffer, defining how the vertex attributes are organized
	};

	class OpenGLIndexBuffer : public rendering::IndexBuffer {
		public:
			OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
			~OpenGLIndexBuffer();

			// Binds the index buffer so that it can be used in rendering.
			virtual void bind() const override;

			// Unbinds the index buffer so that it is no longer used in rendering.
			virtual void unbind() const override;

			// Returns the number of indices in the index buffer.
			virtual uint32_t getCount() const override { return m_Count; }

		private:
			uint32_t m_RendererId; // OpenGL renderer ID for the index buffer
			uint32_t m_Count; // Number of indices in the index buffer
	};
}