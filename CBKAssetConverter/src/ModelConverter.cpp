#include "ModelConverter.h"
#include "TextureConverter.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <print>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace cbk::ac {

	struct CollectedMesh {
		std::vector<ModelConverter::Vertex> vertices;
		std::vector<uint32_t> indices;
	};

	struct TextureRef {
		ModelConverter::TextureType type;
		std::string relativePath; // relative to model directory, with .cbkt extension
	};

	struct PropertyRef {
		uint32_t key;
		float value;
	};

	static void collectMeshes(aiNode* node, const aiScene* scene, std::vector<CollectedMesh>& meshes) {
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* aiM = scene->mMeshes[node->mMeshes[i]];
			CollectedMesh cm;

			for (unsigned int v = 0; v < aiM->mNumVertices; v++) {
				ModelConverter::Vertex vert{};
				vert.px = aiM->mVertices[v].x;
				vert.py = aiM->mVertices[v].y;
				vert.pz = aiM->mVertices[v].z;

				if (aiM->HasNormals()) {
					vert.nx = aiM->mNormals[v].x;
					vert.ny = aiM->mNormals[v].y;
					vert.nz = aiM->mNormals[v].z;
				}

				if (aiM->mTextureCoords[0]) {
					vert.tx = aiM->mTextureCoords[0][v].x;
					vert.ty = aiM->mTextureCoords[0][v].y;
				}

				if (aiM->HasTangentsAndBitangents()) {
					vert.tanx = aiM->mTangents[v].x;
					vert.tany = aiM->mTangents[v].y;
					vert.tanz = aiM->mTangents[v].z;
				}

				cm.vertices.push_back(vert);
			}

			for (unsigned int f = 0; f < aiM->mNumFaces; f++) {
				aiFace& face = aiM->mFaces[f];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
					cm.indices.push_back(face.mIndices[j]);
			}

			meshes.push_back(std::move(cm));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
			collectMeshes(node->mChildren[i], scene, meshes);
	}

	static void collectTextures(const aiScene* scene, const std::string& modelDir,
	                             std::vector<TextureRef>& textures) {
		std::vector<std::string> seen;

		auto tryAdd = [&](aiMaterial* mat, aiTextureType aiType, ModelConverter::TextureType cbkType) {
			for (unsigned int i = 0; i < mat->GetTextureCount(aiType); i++) {
				aiString str;
				mat->GetTexture(aiType, i, &str);
				std::string filename(str.C_Str());

				// Normalize Windows backslashes
				std::ranges::replace(filename, '\\', '/');

				bool already = false;
				for (const auto& s : seen) {
					if (s == filename) { already = true; break; }
				}
				if (already) continue;

				seen.push_back(filename);

				std::string srcPath = modelDir + "/" + filename;
				TextureConverter::convert(srcPath);

				std::filesystem::path rel(filename);
				rel.replace_extension(".cbkt");

				textures.push_back({ cbkType, rel.string() });
			}
		};

		bool hasNormal = false, hasMetalRough = false, hasAO = false;

		for (unsigned int m = 0; m < scene->mNumMaterials; m++) {
			aiMaterial* mat = scene->mMaterials[m];

			// Phong texture types
			using TT = ModelConverter::TextureType;
			tryAdd(mat, aiTextureType_DIFFUSE, TT::Diffuse);
			tryAdd(mat, aiTextureType_SPECULAR, TT::Specular);

			// PBR texture types
			tryAdd(mat, aiTextureType_BASE_COLOR, TT::Diffuse);
			tryAdd(mat, aiTextureType_NORMALS, TT::Normal);
			tryAdd(mat, aiTextureType_METALNESS, TT::MetalRoughness);
			tryAdd(mat, aiTextureType_AMBIENT_OCCLUSION, TT::AO);

			if (mat->GetTextureCount(aiTextureType_NORMALS) > 0) hasNormal = true;
			if (mat->GetTextureCount(aiTextureType_METALNESS) > 0) hasMetalRough = true;
			if (mat->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) > 0) hasAO = true;
		}

		// PBR discovery heuristic: if we found a Diffuse/Albedo but no PBR textures,
		// try to find companion textures by naming convention (_N, _M, _R, _AO)
		if (!textures.empty() && !hasNormal && !hasMetalRough && !hasAO) {
			// Find the diffuse/albedo entry
			std::string diffuseRel;
			for (const auto& tex : textures) {
				if (tex.type == ModelConverter::TextureType::Diffuse) { diffuseRel = tex.relativePath; break; }
			}

			if (!diffuseRel.empty()) {
				// Convert .cbkt back to original extension to find the base
				std::filesystem::path diffPath(diffuseRel);
				diffPath.replace_extension(""); // remove .cbkt
				std::string stem = diffPath.string();

				// Strip the _A suffix to get the base prefix
				std::string basePrefix;
				if (stem.size() >= 2 && stem.substr(stem.size() - 2) == "_A")
					basePrefix = stem.substr(0, stem.size() - 2);

				if (!basePrefix.empty()) {
					// Look for each PBR companion with common extensions
					auto findTexture = [&](const std::string& suffix) -> std::string {
						for (const char* ext : { ".tga", ".png", ".jpg", ".jpeg", ".TGA", ".PNG", ".JPG" }) {
							std::string candidate = modelDir + "/" + basePrefix + suffix + ext;
							if (std::filesystem::exists(candidate))
								return candidate;
						}
						return "";
					};

					// Normal map
					std::string normalPath = findTexture("_N");
					if (!normalPath.empty()) {
						TextureConverter::convert(normalPath);
						std::filesystem::path rel(basePrefix + "_N");
						rel.replace_extension(".cbkt");
						textures.push_back({ ModelConverter::TextureType::Normal, rel.string() });
						std::println("Discovered Normal: {}", normalPath);
					}

					// Metalness + Roughness -> pack into combined MetalRough
					std::string metalPath = findTexture("_M");
					std::string roughPath = findTexture("_R");
					if (!metalPath.empty() && !roughPath.empty()) {
						std::filesystem::path outRel(basePrefix + "_MR");
						outRel.replace_extension(".cbkt");
						std::string outPath = modelDir + "/" + outRel.string();
						TextureConverter::packMetalRough(metalPath, roughPath, outPath);
						textures.push_back({ ModelConverter::TextureType::MetalRoughness, outRel.string() });
						std::println("Discovered Metal+Rough: {} + {}", metalPath, roughPath);
					}

					// AO
					std::string aoPath = findTexture("_AO");
					if (!aoPath.empty()) {
						TextureConverter::convert(aoPath);
						std::filesystem::path rel(basePrefix + "_AO");
						rel.replace_extension(".cbkt");
						textures.push_back({ ModelConverter::TextureType::AO, rel.string() });
						std::println("Discovered AO: {}", aoPath);
					}
				}
			}
		}
	}

	static void collectProperties(const aiScene* scene, std::vector<PropertyRef>& properties) {
		if (scene->mNumMaterials == 0) return;

		// Extract from first material (shared material model)
		aiMaterial* mat = scene->mMaterials[0];

		float shininess;
		if (mat->Get(AI_MATKEY_SHININESS, shininess) == aiReturn_SUCCESS)
			properties.push_back({ 1, shininess });

		float metallic;
		if (mat->Get(AI_MATKEY_METALLIC_FACTOR, metallic) == aiReturn_SUCCESS)
			properties.push_back({ 2, metallic });

		float roughness;
		if (mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness) == aiReturn_SUCCESS)
			properties.push_back({ 3, roughness });

		aiColor4D baseColor;
		if (mat->Get(AI_MATKEY_BASE_COLOR, baseColor) == aiReturn_SUCCESS) {
			properties.push_back({ 4, baseColor.r });
			properties.push_back({ 5, baseColor.g });
			properties.push_back({ 6, baseColor.b });
		}
	}

	void ModelConverter::convert(std::string_view path) {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(
			path.data(),
			aiProcess_Triangulate | aiProcess_GenSmoothNormals |
			aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) {
			std::println("Failed to load model: {} - {}", path, importer.GetErrorString());
			return;
		}

		std::string modelDir(path.substr(0, path.find_last_of('/')));

		std::vector<CollectedMesh> meshes;
		collectMeshes(scene->mRootNode, scene, meshes);

		std::vector<TextureRef> textures;
		collectTextures(scene, modelDir, textures);

		std::vector<PropertyRef> properties;
		collectProperties(scene, properties);

		std::filesystem::path outputPath(path);
		outputPath.replace_extension(".cbkm");

		std::ofstream out(outputPath, std::ios::binary);
		if (!out) {
			std::println("Failed to create output file: {}", outputPath.string());
			return;
		}

		// Header
		ModelHeader header {
			.numMeshes = static_cast<uint32_t>(meshes.size()),
			.numTextures = static_cast<uint32_t>(textures.size()),
			.numProperties = static_cast<uint32_t>(properties.size())
		};
		out.write(reinterpret_cast<const char*>(&header), sizeof(header));

		// Texture table
		for (const auto& tex : textures) {
			TextureEntry entry{
				.type = tex.type,
				.pathLength = static_cast<uint32_t>(tex.relativePath.size())
			};
			out.write(reinterpret_cast<const char*>(&entry), sizeof(entry));
			out.write(tex.relativePath.data(), entry.pathLength);
		}

		// Property table
		for (const auto& prop : properties) {
			PropertyEntry entry{
				.key = prop.key,
				.value = prop.value
			};
			out.write(reinterpret_cast<const char*>(&entry), sizeof(entry));
		}

		// Mesh data
		for (const auto& mesh : meshes) {
			MeshHeader mh{
				.numVertices = static_cast<uint32_t>(mesh.vertices.size()),
				.numIndices = static_cast<uint32_t>(mesh.indices.size())
			};		
			out.write(reinterpret_cast<const char*>(&mh), sizeof(mh));
			out.write(reinterpret_cast<const char*>(mesh.vertices.data()),
			          mesh.vertices.size() * sizeof(Vertex));
			out.write(reinterpret_cast<const char*>(mesh.indices.data()),
			          mesh.indices.size() * sizeof(uint32_t));
		}

		std::println("Converted model: {} -> {} ({} meshes, {} textures, {} properties)",
		             path, outputPath.string(), meshes.size(), textures.size(), properties.size());
	}
} // namespace cbk::ac
