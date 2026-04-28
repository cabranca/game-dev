#pragma once

#include <Cabrankengine/Renderer/Materials/PhongMaterial.h>
#include <Cabrankengine/Renderer/Materials/PBRMaterial.h>

#include "Mesh.h"

#include <Common/BinaryFormats.h>
#include <type_traits>

namespace cbk::scene {

	using cbk::common::ModelHeader;
	using cbk::common::TextureEntry;
	using cbk::common::PropertyEntry;
	using cbk::common::MeshHeader;

	template<typename TMaterial>
	class Model {
	  public:
		Model(const std::string& path, const Ref<TMaterial>& material = cbk::createRef<TMaterial>()) : m_Material(material) {
			std::error_code ec;
			if (!std::filesystem::exists(path, ec)) {
				CBK_CORE_ERROR("Cannot find model file {0} - Error: {1}", path, ec.message());
				return;
			}

			std::ifstream file(path, std::ios::binary);
			if (!file) {
				CBK_CORE_ERROR("Cannot open model file {0}", path);
				return;
			}

			cbk::common::ModelHeader header;
			if (!file.read(reinterpret_cast<char*>(&header), sizeof(cbk::common::ModelHeader))) {
				CBK_CORE_ERROR("Cannot read model header from {0}", path);
				return;
			}
			if (header.magic != 0x43424B4D) {
				CBK_CORE_ERROR("Invalid model file {0} - .cbkm expected!", path);
				return;
			}

			std::string directory = path.substr(0, path.find_last_of('/'));

			// Read texture table
			for (uint32_t i = 0; i < header.numTextures; i++) {
				TextureEntry entry;
				if (!file.read(reinterpret_cast<char*>(&entry), sizeof(TextureEntry))) {
					CBK_CORE_ERROR("Failed to read texture entry {0} from {1}", i, path);
					return;
				}

				std::string texPath(entry.pathLength, '\0');
				if (!file.read(texPath.data(), entry.pathLength)) {
					CBK_CORE_ERROR("Failed to read texture path {0} from {1}", i, path);
					return;
				}

				std::string fullTexPath = directory + "/" + texPath;
				auto texture = rendering::Texture2D::create(fullTexPath);

				applyTexture(entry.type, texture);
			}

			// Read property table
			for (uint32_t i = 0; i < header.numProperties; i++) {
				PropertyEntry prop;
				if (!file.read(reinterpret_cast<char*>(&prop), sizeof(PropertyEntry))) {
					CBK_CORE_ERROR("Failed to read property entry {0} from {1}", i, path);
					return;
				}

				applyProperty(prop.key, prop.value);
			}

			// Read meshes
			for (uint32_t i = 0; i < header.numMeshes; i++) {
				MeshHeader mh;
				if (!file.read(reinterpret_cast<char*>(&mh), sizeof(MeshHeader))) {
					CBK_CORE_ERROR("Failed to read mesh header {0} from {1}", i, path);
					return;
				}

				std::vector<cbk::common::Vertex> rawVertices(mh.numVertices);
				if (!file.read(reinterpret_cast<char*>(rawVertices.data()), mh.numVertices * sizeof(cbk::common::Vertex))) {
					CBK_CORE_ERROR("Failed to read mesh vertices {0} from {1}", i, path);
					return;
				}

				std::vector<uint32_t> indices(mh.numIndices);
				if (!file.read(reinterpret_cast<char*>(indices.data()), mh.numIndices * sizeof(uint32_t))) {
					CBK_CORE_ERROR("Failed to read mesh indices {0} from {1}", i, path);
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

		static Ref<Model<TMaterial>> create(const std::string& path, const Ref<TMaterial>& material = cbk::createRef<TMaterial>()) {
			return createRef<Model<TMaterial>>(path, material);
		}

	  private:
		void applyTexture(cbk::common::TextureType type, const Ref<rendering::Texture2D>& texture) {
			if constexpr (std::is_same_v<TMaterial, rendering::PhongMaterial>) {
				if (type == cbk::common::TextureType::Diffuse) m_Material->setDiffuseMap(texture);
				else if (type == cbk::common::TextureType::Specular) m_Material->setSpecularMap(texture);
			} else if constexpr (std::is_same_v<TMaterial, rendering::PBRMaterial>) {
				if (type == cbk::common::TextureType::Diffuse) m_Material->setAlbedoMap(texture);
				else if (type == cbk::common::TextureType::Normal) m_Material->setNormalMap(texture);
				else if (type == cbk::common::TextureType::MetalRoughness) m_Material->setMetalRoughMap(texture);
				else if (type == cbk::common::TextureType::AO) m_Material->setAOMap(texture);
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
} // namespace cbk::scene