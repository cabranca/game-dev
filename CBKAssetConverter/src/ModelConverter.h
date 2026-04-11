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
			float tanx, tany, tanz; // tangent
		};

	  private:
		struct ModelHeader {
			uint32_t magic = 0x43424B4D; // "CBKM" (Cabrankengine Model)
			uint32_t version = 2;
			uint32_t numMeshes;
			uint32_t numTextures;
			uint32_t numProperties;
		};

		// Texture types:
		// 1 = Diffuse / Albedo (BaseColor)
		// 2 = Specular
		// 3 = Normal
		// 4 = MetalRoughness (packed: B=metal, G=roughness)
		// 5 = AO (Ambient Occlusion)
		struct TextureEntry {
			uint32_t type;
			uint32_t pathLength;
			// followed by char[pathLength]
		};

		// Property keys:
		// 1 = Shininess, 2 = Metalness, 3 = Roughness
		// 4 = BaseColorR, 5 = BaseColorG, 6 = BaseColorB
		struct PropertyEntry {
			uint32_t key;
			float value;
		};

		struct MeshHeader {
			uint32_t numVertices;
			uint32_t numIndices;
			// followed by Vertex[numVertices] then uint32_t[numIndices]
		};
	};
} // namespace cbk::ac
