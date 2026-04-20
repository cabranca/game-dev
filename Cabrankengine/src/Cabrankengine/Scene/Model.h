#pragma once

#include <Cabrankengine/Renderer/Materials/PhongMaterial.h>
#include <Cabrankengine/Renderer/Materials/PBRMaterial.h>

#include "Mesh.h"

#include <type_traits>

namespace cabrankengine::scene {

	struct ModelHeader {
		uint32_t magic = 0x43424B4D; // "CBKM" (Cabrankengine Model)
		uint32_t version = 2;
		uint32_t numMeshes;
		uint32_t numTextures;
		uint32_t numProperties;
	};

	enum class ModelTextureType : uint32_t {
		Diffuse = 1,
		Specular = 2,
		Normal = 3,
		MetalRoughness = 4,
		AO = 5
	};

	struct ModelTextureEntry {
		ModelTextureType type;
		uint32_t pathLength;
	};

	struct ModelPropertyEntry {
		uint32_t key;
		float value;
	};

	struct ModelVertex {
		float px, py, pz;
		float nx, ny, nz;
		float tx, ty;
		float tanx, tany, tanz;
	};

	struct ModelMeshHeader {
		uint32_t numVertices;
		uint32_t numIndices;
	};

	template<typename TMaterial>
	class Model {
	  public:
		Model(const std::string& path, const Ref<TMaterial>& material) : m_Material(material) {
			std::error_code ec;
			if (!std::filesystem::exists(path, ec)) {
				CE_CORE_ERROR("Cannot find model file {0} - Error: {1}", path, ec.message());
				return;
			}

			std::ifstream file(path, std::ios::binary);
			if (!file) {
				CE_CORE_ERROR("Cannot open model file {0}", path);
				return;
			}

			ModelHeader header;
			if (!file.read(reinterpret_cast<char*>(&header), sizeof(ModelHeader))) {
				CE_CORE_ERROR("Cannot read model header from {0}", path);
				return;
			}
			if (header.magic != 0x43424B4D) {
				CE_CORE_ERROR("Invalid model file {0} - .cbkm expected!", path);
				return;
			}

			std::string directory = path.substr(0, path.find_last_of('/'));

			// Read texture table
			for (uint32_t i = 0; i < header.numTextures; i++) {
				ModelTextureEntry entry;
				if (!file.read(reinterpret_cast<char*>(&entry), sizeof(ModelTextureEntry))) {
					CE_CORE_ERROR("Failed to read texture entry {0} from {1}", i, path);
					return;
				}

				std::string texPath(entry.pathLength, '\0');
				if (!file.read(texPath.data(), entry.pathLength)) {
					CE_CORE_ERROR("Failed to read texture path {0} from {1}", i, path);
					return;
				}

				std::string fullTexPath = directory + "/" + texPath;
				auto texture = rendering::Texture2D::create(fullTexPath);

				applyTexture(entry.type, texture);
			}

			// Read property table
			for (uint32_t i = 0; i < header.numProperties; i++) {
				ModelPropertyEntry prop;
				if (!file.read(reinterpret_cast<char*>(&prop), sizeof(ModelPropertyEntry))) {
					CE_CORE_ERROR("Failed to read property entry {0} from {1}", i, path);
					return;
				}

				applyProperty(prop.key, prop.value);
			}

			// Read meshes
			for (uint32_t i = 0; i < header.numMeshes; i++) {
				ModelMeshHeader mh;
				if (!file.read(reinterpret_cast<char*>(&mh), sizeof(ModelMeshHeader))) {
					CE_CORE_ERROR("Failed to read mesh header {0} from {1}", i, path);
					return;
				}

				std::vector<ModelVertex> rawVertices(mh.numVertices);
				if (!file.read(reinterpret_cast<char*>(rawVertices.data()), mh.numVertices * sizeof(ModelVertex))) {
					CE_CORE_ERROR("Failed to read mesh vertices {0} from {1}", i, path);
					return;
				}

				std::vector<uint32_t> indices(mh.numIndices);
				if (!file.read(reinterpret_cast<char*>(indices.data()), mh.numIndices * sizeof(uint32_t))) {
					CE_CORE_ERROR("Failed to read mesh indices {0} from {1}", i, path);
					return;
				}

				std::vector<Vertex> vertices;
				vertices.reserve(mh.numVertices);
				for (const auto& rv : rawVertices) {
					Vertex v;
					v.position = { rv.px, rv.py, rv.pz };
					v.normal = { rv.nx, rv.ny, rv.nz };
					v.texCoords = { rv.tx, rv.ty };
					v.tangent = { rv.tanx, rv.tany, rv.tanz };
					vertices.push_back(std::move(v));
				}

				m_Meshes.emplace_back(std::move(vertices), std::move(indices), m_Material);
			}
		}

		void draw(const math::Mat4& transform = math::identityMat()) {
			for (auto& mesh : m_Meshes)
				mesh.draw(transform);
		}

	  private:
		void applyTexture(ModelTextureType type, const Ref<rendering::Texture2D>& texture) {
			if constexpr (std::is_same_v<TMaterial, rendering::PhongMaterial>) {
				if (type == ModelTextureType::Diffuse) m_Material->setDiffuseMap(texture);
				else if (type == ModelTextureType::Specular) m_Material->setSpecularMap(texture);
			} else if constexpr (std::is_same_v<TMaterial, rendering::PBRMaterial>) {
				if (type == ModelTextureType::Diffuse) m_Material->setAlbedoMap(texture);
				else if (type == ModelTextureType::Normal) m_Material->setNormalMap(texture);
				else if (type == ModelTextureType::MetalRoughness) m_Material->setMetalRoughMap(texture);
				else if (type == ModelTextureType::AO) m_Material->setAOMap(texture);
			}
		}

		void applyProperty(uint32_t key, float value) {
			if constexpr (std::is_same_v<TMaterial, rendering::PhongMaterial>) {
				if (key == 1) m_Material->setShininess(value);
			} else if constexpr (std::is_same_v<TMaterial, rendering::PBRMaterial>) {
				if (key == 2) m_Material->setMetalness(value);
				else if (key == 3) m_Material->setRoughness(value);
				else if (key == 4) m_BaseColorR = value;
				else if (key == 5) m_BaseColorG = value;
				else if (key == 6) {
					m_Material->setAlbedoColor({ m_BaseColorR, m_BaseColorG, value });
				}
			}
		}

		std::vector<Mesh> m_Meshes;
		Ref<TMaterial> m_Material;
		float m_BaseColorR = 1.f;
		float m_BaseColorG = 1.f;
	};
} // namespace cabrankengine::scene