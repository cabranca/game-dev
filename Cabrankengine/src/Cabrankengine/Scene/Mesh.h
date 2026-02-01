#pragma once

#include <Cabrankengine/Math/MatrixFactory.h>
#include <Cabrankengine/Renderer/Materials/Material.h>
#include <Cabrankengine/Renderer/Shader.h>
#include <Cabrankengine/Renderer/Texture.h>
#include <Cabrankengine/Renderer/VertexArray.h>

namespace cabrankengine::scene {

	struct Vertex {
		math::Vector3 position;
		math::Vector3 normal;
		math::Vector2 texCoords;
	};

	enum class TextureType { None = 0, Diffuse, Specular, Normal, Height, Ambient };

	class Mesh {
	  public:
		Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Ref<rendering::Material> material);
		void draw(const math::Mat4& transform);

	  private:
		Ref<rendering::VertexArray> m_VertexArray;
		Ref<rendering::Material> m_Material;
	};
} // namespace cabrankengine::scene