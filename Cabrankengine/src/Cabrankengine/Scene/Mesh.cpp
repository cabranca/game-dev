#include "Mesh.h"

#include <Cabrankengine/Math/MatrixFactory.h>
#include <Cabrankengine/Renderer/Renderer.h>
#include <Cabrankengine/Renderer/Buffer.h>

namespace cabrankengine::scene {

	static std::string textureTypeToUniformName(TextureType type) {
		switch (type) {
		case TextureType::Diffuse:
			return "texture_diffuse";
		case TextureType::Specular:
			return "texture_specular";
		case TextureType::Normal:
			return "texture_normal";
		case TextureType::Height:
			return "texture_height";
		case TextureType::Ambient:
			return "texture_ambient";	
		}
        return "u_Unknown";  
	}

	using namespace rendering;

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureWrapper> textures)
	    : m_Textures(textures) {
		m_VertexArray = VertexArray::create();

		auto vertexBuffer = VertexBuffer::create(vertices.data(), vertices.size() * sizeof(Vertex));
		vertexBuffer->setLayout(
		    { { ShaderDataType::Float3, "pos" }, { ShaderDataType::Float3, "normal" }, { ShaderDataType::Float2, "texCoords" } });
		m_VertexArray->addVertexBuffer(vertexBuffer);

		auto indexBuffer = IndexBuffer::create(indices.data(), indices.size());
		m_VertexArray->setIndexBuffer(indexBuffer);
	}

	void Mesh::draw(const Ref<Shader>& shader) const {
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		unsigned int ambientNr = 1;

		for (unsigned int i = 0; i < m_Textures.size(); i++) {
			m_Textures[i].texture->bind(i);

			// Retrieve texture number (the N in diffuse_textureN)
			std::string number;
			std::string name = textureTypeToUniformName(m_Textures[i].type);

			switch (m_Textures[i].type) {
			case TextureType::Diffuse:
				number = std::to_string(diffuseNr++);
				break;
			case TextureType::Specular:
				number = std::to_string(specularNr++);
				break;
			case TextureType::Normal:
				number = std::to_string(normalNr++);
				break;
			case TextureType::Height:
				number = std::to_string(heightNr++);
				break;
			case TextureType::Ambient:
				number = std::to_string(ambientNr++);
				break;
			case TextureType::None:
				break;
			}

			shader->setInt(name + number, i);
		}

		shader->bind();
		m_Textures[0].texture->bind();
		Renderer::submit(shader, m_VertexArray, math::identityMat());
	}
} // namespace cabrankengine::scene
