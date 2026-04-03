#include <pch.h>
#include "Model.h"

namespace cabrankengine::scene {

	using namespace rendering;

	Model::Model(const std::string& path, const Ref<PhongMaterial>& material) : m_Material(material) {
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
		if (header.magic != 0x43424B4D) { // "CBKM"
			CE_CORE_ERROR("Invalid model file {0} - .cbkm expected!", path);
			return;
		}

		std::string directory = path.substr(0, path.find_last_of('/'));

		// Read texture table and apply to material
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
			auto texture = Texture2D::create(fullTexPath);

			if (entry.type == 1)
				m_Material->setDiffuseMap(texture);
			else if (entry.type == 2)
				m_Material->setSpecularMap(texture);
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

			// Convert ModelVertex -> Vertex
			std::vector<Vertex> vertices;
			vertices.reserve(mh.numVertices);
			for (const auto& rv : rawVertices) {
				Vertex v;
				v.position = { rv.px, rv.py, rv.pz };
				v.normal = { rv.nx, rv.ny, rv.nz };
				v.texCoords = { rv.tx, rv.ty };
				vertices.push_back(std::move(v));
			}

			m_Meshes.emplace_back(std::move(vertices), std::move(indices), m_Material);
		}
	}

	void Model::draw(const math::Mat4& transform) {
		for (auto& mesh: m_Meshes)
			mesh.draw(transform);
	}

} // namespace cabrankengine::scene
