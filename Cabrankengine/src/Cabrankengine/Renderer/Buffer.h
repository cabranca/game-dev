#pragma once

#include <cstdint>
#include <Cabrankengine/Core/Logger.h>
#include <string>
#include <vector>


namespace cabrankengine {

	enum class ShaderDataType { None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool };

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

	struct BufferElement {
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement() {} // TODO: initialize.

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) 
			: Name(name), Type(type), Size(shaderDataTypeSize(type)), Offset(0), Normalized(normalized) { }
		
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

	class BufferLayout {
	public:
		BufferLayout() {} // TODO: initialize

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements), m_Stride(0) {
				calculateOffsetsAndStride();
			}

		inline const std::vector<BufferElement>& getElements() const { return m_Elements; }
		inline uint32_t getStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void calculateOffsetsAndStride() {
			uint32_t offset = 0;
			m_Stride = 0;

			for (auto& element : m_Elements) {
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride;
	};
	
	class VertexBuffer {
	public:
		virtual ~VertexBuffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual const BufferLayout& getLayout() const = 0;
		virtual void setLayout(const BufferLayout& layout) = 0;

		static VertexBuffer* create(float* vertices, uint32_t size);
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t getCount() const = 0;

		static IndexBuffer* create(uint32_t* indices, uint32_t count);
	};
}