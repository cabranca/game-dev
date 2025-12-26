#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Cabrankengine/Renderer/Materials/PhongMaterial.h>

#include "Mesh.h"

namespace cabrankengine::scene {

	class Model {
	  public:
		Model(const std::string& path, const Ref<rendering::PhongMaterial>& defaultShader);

		void draw(const math::Mat4& transform = math::identityMat());

	  private:
		std::vector<Mesh> m_Meshes;
		std::string m_Directory;
		Ref<rendering::PhongMaterial> m_Material;
		std::vector<Ref<rendering::Texture2D>> m_TexturesLoaded;

		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Ref<rendering::Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
	};
} // namespace cabrankengine::scene