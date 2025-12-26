#pragma once

#include <Cabrankengine/Renderer/Texture.h>
#include <Cabrankengine/Renderer/Shader.h>
#include <Cabrankengine/Renderer/VertexArray.h>

namespace cabrankengine::scene {

	class DefaultLibrary {
	  public:
		static void init();

		static Ref<rendering::Texture2D> getWhiteTexture();
		static Ref<rendering::Texture2D> getBlackTexture();
		static Ref<rendering::Texture2D> getFlatNormalTexture();
		static Ref<rendering::Shader> getErrorShader();
		static Ref<rendering::VertexArray> getCube();
		static Ref<rendering::VertexArray> getQuad();

	  private:
		inline static rendering::TextureSpecification s_Specs = { 1, 1, rendering::ImageFormat::RGBA8 };
		inline static Ref<rendering::Texture2D> s_WhiteTexture = nullptr;
		inline static Ref<rendering::Texture2D> s_BlackTexture = nullptr;
		inline static Ref<rendering::Texture2D> s_FlatNormalTexture = nullptr;

		inline static Ref<rendering::Shader> s_ErrorShader = nullptr;

		inline static Ref<rendering::VertexArray> s_CubeVA = nullptr;
		inline static Ref<rendering::VertexArray> s_QuadVA = nullptr;

		static void setupTexture(Ref<rendering::Texture2D>& tex, uint32_t data);
		static void setupQuad();
		static void setupCube();
	};
} // namespace cabrankengine::scene