#include "TextureConverter.h"

#include <filesystem>
#include <fstream>
#include <print>

#include <stb_image.h>

namespace cbk::ac {

	void TextureConverter::convert(std::string_view path) {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;

		data = stbi_load(path.data(), &width, &height, &channels, 0);

		if (!data)
		{
			std::println("Failed to load texture: {}", path);
			return;
		}

		uint32_t dataSize = static_cast<uint32_t>(width) * height * channels;

		TextureHader header{
			.width = static_cast<uint32_t>(width),
			.height = static_cast<uint32_t>(height),
			.channels = static_cast<uint32_t>(channels),
			.dataSize = dataSize
		};

		std::filesystem::path outputPath(path);
		outputPath.replace_extension(".cbkt");

		std::ofstream out(outputPath, std::ios::binary);
		if (!out)
		{
			std::println("Failed to create output file: {}", outputPath.string());
			stbi_image_free(data);
			return;
		}

		out.write(reinterpret_cast<const char*>(&header), sizeof(header));
		out.write(reinterpret_cast<const char*>(data), dataSize);

		stbi_image_free(data);
		std::println("Converted: {} -> {}", path, outputPath.string());
	}
} // namespace cbk::ac