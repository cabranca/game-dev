#pragma once

#include <memory>
#include <vector>
#include <Cabrankengine/Core/Core.h>

namespace cabrankengine {

	// Forward declarations
	class VertexBuffer; // VertexBuffer is a class that represents a buffer containing vertex data.
	class IndexBuffer; // IndexBuffer is a class that represents a buffer containing index data for indexed rendering.

	// VertexArray is an interface that represents a collection of vertex buffers and an index buffer.
	class VertexArray {
		public:
			virtual ~VertexArray() {}

			// Binds the vertex array for use in rendering.
			virtual void bind() const = 0;

			// Unbinds the vertex array, stopping its use in rendering.
			virtual void unbind() const = 0;

			// Adds a vertex buffer to the vertex array.
			virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;

			// Sets the index buffer for the vertex array.
			virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

			// Returns the vertex buffers in the vertex array.
			virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const = 0;

			// Returns the index buffer in the vertex array.
			virtual const Ref<IndexBuffer>& getIndexBuffer() const = 0;

			// Creates a new VertexArray instance based on the current renderer API.
			static Ref<VertexArray> create();
	};
}