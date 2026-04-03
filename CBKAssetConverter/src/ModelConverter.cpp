#include "ModelConverter.h"
#include "TextureConverter.h"

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
		uint32_t type; // 1 = Diffuse, 2 = Specular
		std::string relativePath; // relative to model directory, with .cbkt extension
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
		// Track which filenames we've already seen to avoid duplicates
		std::vector<std::string> seen;

		auto tryAdd = [&](aiMaterial* mat, aiTextureType aiType, uint32_t cbkType) {
			for (unsigned int i = 0; i < mat->GetTextureCount(aiType); i++) {
				aiString str;
				mat->GetTexture(aiType, i, &str);
				std::string filename(str.C_Str());

				bool already = false;
				for (const auto& s : seen) {
					if (s == filename) { already = true; break; }
				}
				if (already) continue;

				seen.push_back(filename);

				// Convert the source texture to .cbkt
				std::string srcPath = modelDir + "/" + filename;
				TextureConverter::convert(srcPath);

				// Store relative path with .cbkt extension
				std::filesystem::path rel(filename);
				rel.replace_extension(".cbkt");

				textures.push_back({ cbkType, rel.string() });
			}
		};

		// Walk all materials in the scene. Last-found wins at load time (matches engine behavior).
		for (unsigned int m = 0; m < scene->mNumMaterials; m++) {
			aiMaterial* mat = scene->mMaterials[m];
			tryAdd(mat, aiTextureType_DIFFUSE, 1);
			tryAdd(mat, aiTextureType_SPECULAR, 2);
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

		// Collect meshes
		std::vector<CollectedMesh> meshes;
		collectMeshes(scene->mRootNode, scene, meshes);

		// Collect & convert textures
		std::vector<TextureRef> textures;
		collectTextures(scene, modelDir, textures);

		// Write .cbkm
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
			.numTextures = static_cast<uint32_t>(textures.size())
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

		std::println("Converted model: {} -> {} ({} meshes, {} textures)",
		             path, outputPath.string(), meshes.size(), textures.size());
	}
} // namespace cbk::ac
