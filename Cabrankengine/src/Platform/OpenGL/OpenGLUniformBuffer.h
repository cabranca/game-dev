#pragma once

#include <Cabrankengine/Renderer/UniformBuffer.h>

namespace cabrankengine::platform::opengl {

	class OpenGLUniformBuffer : public rendering::UniformBuffer {
	  public:
		OpenGLUniformBuffer(uint32_t size, uint32_t binding);
		~OpenGLUniformBuffer();

		void setData(const void* data, uint32_t size, uint32_t offset = 0) override;

	  private:
		uint32_t m_RendererID;
	};
} // namespace cabrankengine::platform::opengl