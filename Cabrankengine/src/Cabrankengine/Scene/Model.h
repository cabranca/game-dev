#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Cabrankengine/Renderer/Shader.h>

#include "Mesh.h"

namespace cabrankengine::scene {

	class Model {
	  public:
		Model(const std::string& path, const Ref<rendering::Shader>& defaultShader);

		void draw();

	  private:
		std::vector<Mesh> m_Meshes;
		std::string m_Directory;
		Ref<rendering::Shader> m_DefaultShader;
		std::vector<Ref<rendering::Texture2D>> m_TexturesLoaded;

		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Ref<rendering::Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
	};
} // namespace cabrankengine::scene