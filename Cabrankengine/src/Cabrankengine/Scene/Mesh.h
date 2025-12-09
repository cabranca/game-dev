#pragma once

#include <Cabrankengine/Math/Vector3.h>
#include <Cabrankengine/Renderer/Shader.h>
#include <Cabrankengine/Renderer/Texture.h>
#include <Cabrankengine/Renderer/VertexArray.h>

namespace cabrankengine::scene {

    struct Vertex {
        math::Vector3 position;
        math::Vector3 normal;
        math::Vector2 texCoords;
    };

    // Maybe the type should be moved to our Texture class
    struct TextureWrapper {
        Ref<rendering::Texture2D> texture;
        std::string type;
    };

	class Mesh {
	  public:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<TextureWrapper> textures);
        void draw(const Ref<rendering::Shader>& shader) const;

	  private:
        Ref<rendering::VertexArray> m_VertexArray;
        std::vector<TextureWrapper> m_Textures;

        void setupMesh();
	};
} // namespace cabrankengine::rendering