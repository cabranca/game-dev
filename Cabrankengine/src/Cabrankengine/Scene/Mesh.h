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

    enum class TextureType {
        None = 0,
        Diffuse,
        Specular,
        Normal,
        Height,
        Ambient
    };

    // For now we are using a string to match the texture type with the uniform name in the shader.
    // Think of alternatives (enum and map to string?)
    struct TextureWrapper {
        Ref<rendering::Texture2D> texture;
        TextureType type;
    };

	class Mesh {
	  public:
        Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<TextureWrapper> textures);
        void draw(const Ref<rendering::Shader>& shader) const;

	  private:
        Ref<rendering::VertexArray> m_VertexArray;
        std::vector<TextureWrapper> m_Textures;
	};
} // namespace cabrankengine::rendering