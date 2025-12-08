#pragma once

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include "Mesh.h"

namespace cabrankengine::rendering {

	class Model {
	  public:
		Model(std::string_view path);

		void draw(const Ref<Shader>& shader);

	  private:
		std::vector<Mesh> m_Meshes;
		std::string_view m_Directory;
		std::vector<TextureWrapper> m_TexturesLoaded; // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
		bool m_GammaCorrection;

		void loadModel(std::string_view path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<TextureWrapper> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
	};

	Ref<Texture2D> TextureFromFile(const char* path, std::string_view directory, bool gamma = false);
} // namespace cabrankengine::rendering