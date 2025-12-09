#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

namespace cabrankengine::scene {

	class Model {
	  public:
		Model(const std::string& path);

		void draw(const Ref<rendering::Shader>& shader);

	  private:
		std::vector<Mesh> m_Meshes;
		std::string m_Directory;
		std::vector<TextureWrapper> m_TexturesLoaded; // Cache of Textures so they're not loaded twice

		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<TextureWrapper> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
	};
} // namespace cabrankengine::scene