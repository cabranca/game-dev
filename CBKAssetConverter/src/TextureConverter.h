#pragma once

#include <cstdint>

#include <string_view>

namespace cbk::ac {

	class TextureConverter {
	  public:
		static void convert(std::string_view path);
		// Pack separate metalness and roughness textures into a combined MetalRough .cbkt
		// Output: R=0, G=roughness, B=metalness, A=255
		static void packMetalRough(std::string_view metalPath, std::string_view roughPath,
		                           std::string_view outputPath);

	  private:
		struct TextureHader {
			uint32_t magic = 0x43424B54; // "CBKT" (Cabrankengine Texture)
			uint32_t version = 1;
			uint32_t width, height, channels;
			uint32_t compressedSize;
			uint32_t uncompressedSize;
		};
	};
} // namespace cbk::ac