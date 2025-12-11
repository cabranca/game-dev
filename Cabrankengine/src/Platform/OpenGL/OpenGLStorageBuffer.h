#pragma once

#include <Cabrankengine/Renderer/StorageBuffer.h>

namespace cabrankengine::platform::opengl {

	class OpenGLStorageBuffer : public rendering::StorageBuffer {
	  public:
		OpenGLStorageBuffer(uint32_t size);
		~OpenGLStorageBuffer() override;
		void setData(const void* data, uint32_t size) override;
		void bind(uint32_t bindingPoint) const override;

	  private:
		uint32_t m_RendererID;
	};
} // namespace cabrankengine::platform::opengl