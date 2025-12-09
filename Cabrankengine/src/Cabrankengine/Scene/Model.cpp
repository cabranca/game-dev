#include "Model.h"

#include <filesystem>

namespace cabrankengine::rendering {

	Model::Model(std::string_view path) {
		loadModel(path);
	}

	void Model::draw(const Ref<Shader>& shader) {
		for (const auto& mesh: m_Meshes)
			mesh.draw(shader);
	}

	void Model::loadModel(std::string_view path) {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path.data(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			CE_CORE_ERROR("Failed to load model from path {0}. {1}", path.data(), importer.GetErrorString());
			return;
		}
		m_Directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
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
		std::vector<TextureWrapper> textures;

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

		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<TextureWrapper> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<TextureWrapper> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		std::vector<TextureWrapper> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal"); // ESTO ESTA MAL
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		std::vector<TextureWrapper> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		return Mesh(vertices, indices, textures);
	}

	std::vector<TextureWrapper> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName) {
		std::vector<TextureWrapper> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;	
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for (unsigned int j = 0; j < m_TexturesLoaded.size(); j++) {
				std::filesystem::path path(m_TexturesLoaded[j].texture->getPath());
                if (!std::strcmp(path.filename().string().c_str(), str.C_Str()) ) {
                    textures.push_back(m_TexturesLoaded[j]);
                    skip = true;
                    break;
                }
            }
            if (skip)
                continue;
            
            TextureWrapper textureWrapper;
            textureWrapper.texture = TextureFromFile(str.C_Str(), m_Directory);
            textureWrapper.type = typeName;
            textures.push_back(textureWrapper);
			m_TexturesLoaded.push_back(textureWrapper);
        }

        return textures;
	}

	Ref<Texture2D> TextureFromFile(const char* path, std::string_view directory, bool gamma) {
		std::string filename = std::string(directory) + "/" + std::string(path);
        return Texture2D::create(filename);
	}
} // namespace cabrankengine::rendering
