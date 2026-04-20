#pragma once

#include <string_view>

#include <Common/BinaryFormats.h>

namespace cbk::ac {

	class TextureConverter {
	  public:
		static void convert(std::string_view path);
		
		// Pack separate metalness and roughness textures into a combined MetalRough .cbkt
		// Output: R=0, G=roughness, B=metalness, A=255
		static void packMetalRough(std::string_view metalPath, std::string_view roughPath,
		                           std::string_view outputPath);

	  private:
		using TextureHeader = cbk::common::TextureHeader;
	};
} // namespace cbk::ac