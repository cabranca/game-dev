#include <pch.h>
#include "Mesh.h"

#include <Cabrankengine/Math/MatrixFactory.h>
#include <Cabrankengine/Renderer/Renderer.h>
#include <Cabrankengine/Renderer/Buffer.h>

namespace cbk::scene {

	using namespace math;
	using namespace rendering;

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Ref<Material> material) : m_Material(material) {
		m_VertexArray = VertexArray::create();

		auto vertexBuffer = VertexBuffer::create(vertices.data(), vertices.size() * sizeof(Vertex));
		vertexBuffer->setLayout(
		    { { ShaderDataType::Float3, "pos" }, { ShaderDataType::Float3, "normal" }, { ShaderDataType::Float2, "texCoords" }, { ShaderDataType::Float3, "tangent" } });
		m_VertexArray->addVertexBuffer(vertexBuffer);

		auto indexBuffer = IndexBuffer::create(indices.data(), indices.size());
		m_VertexArray->setIndexBuffer(indexBuffer);
	}

	void Mesh::draw(const math::Mat4& transform) {
		Renderer::submit(m_Material, m_VertexArray, transform);
	}
} // namespace cbk::scene
