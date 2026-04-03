#pragma once

#include <Cabrankengine/Renderer/Materials/PhongMaterial.h>

#include "Mesh.h"

namespace cabrankengine::scene {

	struct ModelHeader {
		uint32_t magic = 0x43424B4D; // "CBKM" (Cabrankengine Model)
		uint32_t version = 1;
		uint32_t numMeshes;
		uint32_t numTextures;
	};

	struct ModelTextureEntry {
		uint32_t type;       // 1 = Diffuse, 2 = Specular
		uint32_t pathLength;
	};

	struct ModelVertex {
		float px, py, pz;
		float nx, ny, nz;
		float tx, ty;
	};

	struct ModelMeshHeader {
		uint32_t numVertices;
		uint32_t numIndices;
	};

	class Model {
	  public:
		Model(const std::string& path, const Ref<rendering::PhongMaterial>& defaultShader);

		void draw(const math::Mat4& transform = math::identityMat());

	  private:
		std::vector<Mesh> m_Meshes;
		Ref<rendering::PhongMaterial> m_Material;
	};
} // namespace cabrankengine::scene