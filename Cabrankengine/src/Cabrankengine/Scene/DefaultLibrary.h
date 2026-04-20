#pragma once

#include <Cabrankengine/Renderer/Texture.h>
#include <Cabrankengine/Renderer/Shader.h>
#include <Cabrankengine/Renderer/VertexArray.h>

namespace cbk::scene {

	class DefaultLibrary {
	  public:
		static void init();

		static Ref<rendering::Texture2D> getWhiteTexture();
		static Ref<rendering::Texture2D> getBlackTexture();
		static Ref<rendering::Texture2D> getFlatNormalTexture();
		static Ref<rendering::Shader> getErrorShader();
		static Ref<rendering::VertexArray> getCube();
		static Ref<rendering::VertexArray> getQuad();
		static Ref<rendering::VertexArray> getSphere();

	  private:
		inline static rendering::TextureSpecification s_Specs = { 1, 1, rendering::ImageFormat::RGBA8 };
		inline static Ref<rendering::Texture2D> s_WhiteTexture = nullptr;
		inline static Ref<rendering::Texture2D> s_BlackTexture = nullptr;
		inline static Ref<rendering::Texture2D> s_FlatNormalTexture = nullptr;

		inline static Ref<rendering::Shader> s_ErrorShader = nullptr;

		inline static Ref<rendering::VertexArray> s_CubeVA = nullptr;
		inline static Ref<rendering::VertexArray> s_QuadVA = nullptr;
		inline static Ref<rendering::VertexArray> s_SphereVA = nullptr;

		static void setupTexture(Ref<rendering::Texture2D>& tex, uint32_t data);
		static void setupQuad();
		static void setupCube();
		static void setupSphere();
	};
} // namespace cbk::scene