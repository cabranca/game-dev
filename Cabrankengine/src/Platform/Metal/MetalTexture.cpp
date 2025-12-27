#include <pch.h>
#include "MetalTexture.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <stb_image.h>

namespace cabrankengine::platform::metal {

	using namespace rendering;

	MetalTexture2D::MetalTexture2D(const TextureSpecification& specification)
		: m_Specification(specification), m_Width(m_Specification.Width), m_Height(m_Specification.Height) {
		CE_PROFILE_FUNCTION();


	}

	MetalTexture2D::MetalTexture2D(const std::string& path) : m_Path(path) {
		CE_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;

		{
			CE_PROFILE_FUNCTION();
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		if (data)
		{
			m_IsLoaded = true;

			m_Width = width;
			m_Height = height;

			stbi_image_free(data);
		}
	}

	MetalTexture2D::MetalTexture2D(const FT_Face& face) {
		// generate texture
	}

	MetalTexture2D::~MetalTexture2D() {
		CE_PROFILE_FUNCTION();

	}

	void MetalTexture2D::setData(void* data, uint32_t size) {
		CE_PROFILE_FUNCTION();

	}

	void MetalTexture2D::bind(uint32_t slot) const {
		CE_PROFILE_FUNCTION();

	}
}
