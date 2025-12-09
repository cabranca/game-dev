#include "Mesh.h"

#include <Cabrankengine/Math/MatrixFactory.h>
#include <Cabrankengine/Renderer/Renderer.h>
#include <Cabrankengine/Renderer/Buffer.h>

namespace cabrankengine::scene {

    using namespace rendering;

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureWrapper> textures)
	    : vertices(vertices), indices(indices), m_Textures(textures) {
		setupMesh();
	}

	void Mesh::draw(const Ref<Shader>& shader) const {
        // bind appropriate textures
        unsigned int diffuseNr  = 1;
        for(unsigned int i = 0; i < m_Textures.size(); i++) {
            m_Textures[i].texture->bind(i);
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string_view name = m_Textures[i].type;
            if(name == "texture_diffuse") {
                number = std::to_string(diffuseNr++);
                shader->setInt(std::string(name) + number, i);
            }            
        }

        shader->bind();
        m_Textures[0].texture->bind();
        Renderer::submit(shader, m_VertexArray, math::identityMat());
    }
	
    void Mesh::setupMesh() {
        m_VertexArray = VertexArray::create();
		
        auto vertexBuffer = VertexBuffer::create(vertices.data(), vertices.size() * sizeof(Vertex));
		vertexBuffer->setLayout({ { ShaderDataType::Float3, "pos" }, { ShaderDataType::Float3, "normal" }, { ShaderDataType::Float2, "texCoords" } });
		m_VertexArray->addVertexBuffer(vertexBuffer);
		
        auto indexBuffer = IndexBuffer::create(indices.data(), indices.size());
		m_VertexArray->setIndexBuffer(indexBuffer);
    }
} // namespace cabrankengnie::rendering
