#pragma once

#include <Cabrankengine/Math/Vector3.h>

#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

namespace cabrankengine::rendering {

    struct Vertex {
        math::Vector3 position;
        math::Vector3 normal;
        math::Vector2 texCoords;
    };

    // Maybe the type should be moved to our Texture class
    struct TextureWrapper {
        Ref<Texture2D> texture;
        std::string type;
    };

	class Mesh {
	  public:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<TextureWrapper> textures);
        void draw(const Ref<Shader>& shader) const;

	  private:
        Ref<VertexArray> m_VertexArray;
        std::vector<TextureWrapper> m_Textures;

        void setupMesh();
	};
} // namespace cabrankengine::rendering