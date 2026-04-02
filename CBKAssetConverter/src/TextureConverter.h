#pragma once

#include <cstdint>

#include <string_view>

namespace cbk::ac {

	class TextureConverter {
	  public:
		static void convert(std::string_view path);

	  private:
		struct TextureHader {
			uint32_t magic = 0x43424B54; // "CBKT" (Cabrankengine Texture)
			uint32_t version = 1;
			uint32_t width, height, channels;
			uint32_t dataSize;
		};
	};
} // namespace cbk::ac