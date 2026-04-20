#pragma once

#include <Cabrankengine/Renderer/StorageBuffer.h>

namespace cbk::platform::metal {

	class MetalStorageBuffer : public rendering::StorageBuffer {
	  public:
		MetalStorageBuffer(uint32_t size);
		~MetalStorageBuffer() override;
		void setData(const void* data, uint32_t size) override;
		void bind(uint32_t bindingPoint) const override;

	  private:
		uint32_t m_RendererID;
	};
} // namespace cbk::platform::metal