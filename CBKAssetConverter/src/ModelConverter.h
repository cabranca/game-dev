#pragma once

#include <cstdint>
#include <string_view>

namespace cbk::ac {

	class ModelConverter {
	  public:
		static void convert(std::string_view path);

		struct Vertex {
			float px, py, pz;    // position
			float nx, ny, nz;    // normal
			float tx, ty;        // texCoords
		};

	  private:
		struct ModelHeader {
			uint32_t magic = 0x43424B4D; // "CBKM" (Cabrankengine Model)
			uint32_t version = 1;
			uint32_t numMeshes;
			uint32_t numTextures;
		};

		struct TextureEntry {
			uint32_t type;       // 1 = Diffuse, 2 = Specular
			uint32_t pathLength;
			// followed by char[pathLength]
		};

		struct MeshHeader {
			uint32_t numVertices;
			uint32_t numIndices;
			// followed by Vertex[numVertices] then uint32_t[numIndices]
		};
	};
} // namespace cbk::ac
