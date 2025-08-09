#pragma once

#include <Cabrankengine/Renderer/VertexArray.h>

namespace cabrankengine {

	class OpenGLVertexArray : public VertexArray {
		public:
			OpenGLVertexArray();
			~OpenGLVertexArray();

			// Binds the vertex array for use in rendering.
			virtual void bind() const override;

			// Unbinds the vertex array, stopping its use in rendering.
			virtual void unbind() const override;

			// Adds a vertex buffer to the vertex array.
			virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
			
			// Sets the index buffer for the vertex array.
			virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

			// Returns the vertex buffers in the vertex array.
			virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const override { return m_VertexBuffers; }
			
			// Returns the index buffer in the vertex array.
			virtual const Ref<IndexBuffer>& getIndexBuffer() const override { return m_IndexBuffer; }

		private:
			std::vector<Ref<VertexBuffer>> m_VertexBuffers; // Vector of vertex buffers in the vertex array
			Ref<IndexBuffer> m_IndexBuffer; // Index buffer in the vertex array

			uint32_t m_RendererId; // Renderer ID for the OpenGL vertex array, used to identify it in the OpenGL context
	};
}