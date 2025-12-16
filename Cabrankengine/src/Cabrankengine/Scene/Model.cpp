#include <pch.h>
#include "Model.h"

namespace cabrankengine::scene {

	using namespace rendering;

	Model::Model(const std::string& path, const Ref<Shader>& defaultShader) : m_DefaultShader(defaultShader) {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
		                                                          aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			CE_CORE_ERROR("Failed to load model from path {0}. {1}", path.data(), importer.GetErrorString());
			return;
		}
		m_Directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
	}

	void Model::draw() {
		for (auto& mesh: m_Meshes)
			mesh.draw();
	}

	void Model::processNode(aiNode* node, const aiScene* scene) {
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(processMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
			processNode(node->mChildren[i], scene);
	}

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;
			vertex.position.x = mesh->mVertices[i].x;
			vertex.position.y = mesh->mVertices[i].y;
			vertex.position.z = mesh->mVertices[i].z;

			if (mesh->HasNormals()) {
				vertex.normal.x = mesh->mNormals[i].x;
				vertex.normal.y = mesh->mNormals[i].y;
				vertex.normal.z = mesh->mNormals[i].z;
			}

			if (mesh->mTextureCoords[0]) {
				vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
				vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
			}

			vertices.push_back(std::move(vertex));
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		Ref<Material> meshMaterial = Material::create(m_DefaultShader);

		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			// Load diffuse (only support 1 for now)
			auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::Diffuse);
			if (!diffuseMaps.empty())
				meshMaterial->setDiffuseMap(diffuseMaps[0]);

			// Load specular (only support 1 for now)
			auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::Specular);
			if (!specularMaps.empty())
				meshMaterial->setSpecularMap(specularMaps[0]);
				
		}

		return Mesh(vertices, indices, meshMaterial);
	}

	std::vector<Ref<Texture2D>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName) {
		std::vector<Ref<Texture2D>> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;
			for (unsigned int j = 0; j < m_TexturesLoaded.size(); j++) {
				std::filesystem::path path(m_TexturesLoaded[j]->getPath());
				if (!std::strcmp(path.filename().string().c_str(), str.C_Str())) {
					textures.push_back(m_TexturesLoaded[j]);
					skip = true;
					break;
				}
			}
			if (skip)
				continue;

			auto texture = Texture2D::create(std::string(m_Directory) + "/" + std::string(str.C_Str()));
			textures.push_back(texture);
			m_TexturesLoaded.push_back(texture);
		}

		return textures;
	}
} // namespace cabrankengine::scene
