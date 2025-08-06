#pragma once

#include <Cabrankengine/Renderer/Texture.h>
#include <glad/glad.h>

namespace cabrankengine {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const TextureSpecification& specification);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual const TextureSpecification& getSpecification() const override { return m_Specification; }

		virtual uint32_t getWidth() const override { return m_Width; }
		virtual uint32_t getHeight() const override { return m_Height; }
		virtual uint32_t getRendererID() const override { return m_RendererID; }

		virtual const std::string& getPath() const override { return m_Path; }

		virtual void setData(void* data, uint32_t size) override;

		virtual void bind(uint32_t slot = 0) const override;

		virtual bool isLoaded() const override { return m_IsLoaded; }

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.getRendererID();
		}
	private:
		TextureSpecification m_Specification;

		std::string m_Path;
		bool m_IsLoaded = false;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};

}
