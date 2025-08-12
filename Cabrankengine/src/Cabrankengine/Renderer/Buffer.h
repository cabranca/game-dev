#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <Cabrankengine/Core/Logger.h>

namespace cabrankengine {

	// ShaderDataType is an enum class that defines the types of data that can be used in shaders.
	enum class ShaderDataType { None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool };

	// Returns the size in bytes of the given ShaderDataType.
	static uint32_t shaderDataTypeSize(ShaderDataType type) {
		switch(type) {
			case ShaderDataType::Float:   return 4;
			case ShaderDataType::Float2:  return 4 * 2;
			case ShaderDataType::Float3:  return 4 * 3;
			case ShaderDataType::Float4:  return 4 * 4;
			case ShaderDataType::Mat3:    return 4 * 3 * 3;
			case ShaderDataType::Mat4:    return 4 * 4 * 4;
			case ShaderDataType::Int:     return 4;
			case ShaderDataType::Int2:    return 4 * 2;
			case ShaderDataType::Int3:    return 4 * 3;
			case ShaderDataType::Int4:    return 4 * 4;
			case ShaderDataType::Bool:    return 1;
		}
		CE_CORE_ASSERT(false, "Unknown Shader Type!");
		return 0;
	}

	// BufferElement is a struct that represents an element in a buffer layout.
	struct BufferElement {
		std::string Name; // Name of the element, used for debugging and identification
		ShaderDataType Type; // Type of the element, defined by ShaderDataType enum
		uint32_t Size; // Size in bytes of the element, calculated based on the ShaderDataType
		uint32_t Offset; // Offset in bytes of the element within the buffer layout, calculated during layout creation
		bool Normalized; // Whether the element should be normalized when used in shaders

		BufferElement() : Name(), Type(ShaderDataType::None), Size(), Offset(), Normalized() {}

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) 
			: Name(name), Type(type), Size(shaderDataTypeSize(type)), Offset(0), Normalized(normalized) { }
		
		// Returns the number of components in the ShaderDataType.
		uint32_t getComponentCount() const {
			switch(Type) {
				case ShaderDataType::Float:   return 1;
				case ShaderDataType::Float2:  return 2;
				case ShaderDataType::Float3:  return 3;
				case ShaderDataType::Float4:  return 4;
				case ShaderDataType::Mat3:    return 3 * 3;
				case ShaderDataType::Mat4:    return 4 * 4;
				case ShaderDataType::Int:     return 1;
				case ShaderDataType::Int2:    return 2;
				case ShaderDataType::Int3:    return 3;
				case ShaderDataType::Int4:    return 4;
				case ShaderDataType::Bool:    return 1;
			}

			CE_CORE_ASSERT(false, "Unknown Shader Type!");
			return 0;
		}
	};

	// BufferLayout is a class that represents the layout of a buffer, containing multiple BufferElements.
	class BufferLayout {
		public:
			BufferLayout() : m_Stride() {}

			BufferLayout(const std::initializer_list<BufferElement>& elements)
				: m_Elements(elements), m_Stride(0) {
					calculateOffsetsAndStride();
				}

			// Returns a vector of BufferElements, representing the layout of the buffer.
			const std::vector<BufferElement>& getElements() const { return m_Elements; }

			// Returns the total stride of the buffer layout, which is the sum of the sizes of all elements.
			uint32_t getStride() const { return m_Stride; }

			// Returns an interator to the beginning of the elements vector.
			std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }

			// Returns an interator to the end of the elements vector.
			std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

			// Returns a const interator to the beginning of the elements vector.
			std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }

			// Returns a const interator to the end of the elements vector.
			std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

		private:
			// Calculates the offsets and stride of each BufferElement in the layout.
			void calculateOffsetsAndStride() {
				uint32_t offset = 0;
				m_Stride = 0;

				for (auto& element : m_Elements) {
					element.Offset = offset;
					offset += element.Size;
					m_Stride += element.Size;
				}
			}

			std::vector<BufferElement> m_Elements; // Vector of BufferElements representing the layout
			uint32_t m_Stride; // Total stride of the buffer layout, calculated as the sum of sizes of all elements
	};
	
	// A Vertex buffer contains vertex data that can be used for rendering.
	// It is used to store the vertex attributes such as position, color, texture coordinates, etc.
	class VertexBuffer {
		public:
			virtual ~VertexBuffer() = default;

			// Binds the vertex buffer so that it can be used in rendering.
			virtual void bind() const = 0;

			// Unbinds the vertex buffer so that it is no longer used in rendering.
			virtual void unbind() const = 0;

			// Returns the structure of the vertex buffer, which is defined by the BufferLayout.
			virtual const BufferLayout& getLayout() const = 0;

			// Sets the layout of the vertex buffer, which defines how the vertex attributes are organized.
			virtual void setLayout(const BufferLayout& layout) = 0;

			// Returns a pointer to the raw vertex data.
			static Ref<VertexBuffer> create(float* vertices, uint32_t size);
	};


	// An IndexBuffer contains indices that define how vertices are connected to form primitives (triangles, lines, etc.).
	// It is used to optimize rendering by reusing vertices instead of duplicating them.
	class IndexBuffer {
		public:
			virtual ~IndexBuffer() = default;

			// Binds the index buffer so that it can be used in rendering.
			virtual void bind() const = 0;

			// Unbinds the index buffer so that it is no longer used in rendering.
			virtual void unbind() const = 0;

			// Returns the number of indices in the index buffer.
			virtual uint32_t getCount() const = 0;

			// Returns a pointer to the raw index data.
			static Ref<IndexBuffer> create(uint32_t* indices, uint32_t count);
	};
}