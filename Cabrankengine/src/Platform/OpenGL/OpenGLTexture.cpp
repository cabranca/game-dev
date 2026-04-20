#include <pch.h>
#include "OpenGLTexture.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <lz4.h>

namespace cbk::platform::opengl {

	using namespace rendering;

	namespace utils {

		static GLenum cbkImageFormatToGLDataFormat(ImageFormat format) {
			switch (format) {
			case ImageFormat::RGB8:
				return GL_RGB;
			case ImageFormat::RGBA8:
				return GL_RGBA;
			}

			CBK_CORE_ASSERT(false, "Invalid image format!");
			return 0;
		}

		static GLenum cbkImageFormatToGLInternalFormat(ImageFormat format) {
			switch (format) {
			case ImageFormat::RGB8:
				return GL_RGB8;
			case ImageFormat::RGBA8:
				return GL_RGBA8;
			}

			CBK_CORE_ASSERT(false, "Invalid image format!");
			return 0;
		}

	} // namespace utils

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
	    : m_Specification(specification), m_Width(m_Specification.Width), m_Height(m_Specification.Height) {
		CBK_PROFILE_FUNCTION();

		m_DataFormat = utils::cbkImageFormatToGLDataFormat(m_Specification.Format);
		m_InternalFormat = utils::cbkImageFormatToGLInternalFormat(m_Specification.Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path) {
		CBK_PROFILE_FUNCTION();

		std::error_code errorCode;
		if (!std::filesystem::exists(path, errorCode))
			CBK_CORE_ERROR("Cannot find the path {0} - Error: {1}", path, errorCode.message());

		const auto size = std::filesystem::file_size(path, errorCode);
		if (errorCode)
			CBK_CORE_ERROR("Size check failure for file {0} - Error: {1}", path, errorCode.message());

		std::ifstream file(path, std::ios::binary);
		if (!file)
			CBK_CORE_ERROR("Cannot open file {0}", path);

		TextureHeader header;
		if (!file.read(reinterpret_cast<char*>(&header), sizeof(TextureHeader)))
			CBK_CORE_ERROR("Cannot read the file {0}", path);
		if (header.magic != 0x43424B54) // "CBKT"
			CBK_CORE_ERROR("Wrong extension of file {0} - .cbk expected!", path);

		std::vector<uint8_t> compressedBuffer(header.compressedSize);
		if (!file.read(reinterpret_cast<char*>(compressedBuffer.data()), compressedBuffer.size()))
			CBK_CORE_ERROR("Cannot read the file {0}", path);
		else {
			std::vector<uint8_t> uncompressedBuffer(header.uncompressedSize);
			int result =
			    LZ4_decompress_safe(reinterpret_cast<const char*>(compressedBuffer.data()),
			                        reinterpret_cast<char*>(uncompressedBuffer.data()), header.compressedSize, header.uncompressedSize);
			if (result < 0) {
				CBK_CORE_ERROR("LZ4 decompression failed for {0}", path);
				return;
			}

			m_Width = header.width;
			m_Height = header.height;

			GLenum internalFormat = 0, dataFormat = 0;
			if (header.channels == 4) {
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			} else if (header.channels == 3) {
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;

			CBK_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, uncompressedBuffer.data());

			m_IsLoaded = true;
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(const FT_Face& face) {
		// generate texture
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GL_R8, face->glyph->bitmap.width, face->glyph->bitmap.rows);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE,
		                    face->glyph->bitmap.buffer);

		// set texture options
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		CBK_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::setData(void* data, uint32_t size) {
		CBK_PROFILE_FUNCTION();

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		CBK_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::bind(uint32_t slot) const {
		CBK_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}
} // namespace cbk::platform::opengl
