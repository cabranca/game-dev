#include "TextureConverter.h"

#include <filesystem>
#include <fstream>
#include <print>
#include <vector>

#include <lz4.h>
#include <stb_image.h>

namespace cbk::ac {

	void TextureConverter::convert(std::string_view path) {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;

		data = stbi_load(path.data(), &width, &height, &channels, 0);

		if (!data) {
			std::println("Failed to load texture: {}", path);
			return;
		}

		uint32_t dataSize = static_cast<uint32_t>(width) * height * channels;

		int compressedCapacity = LZ4_compressBound(dataSize);
		std::vector<char> compressedData(compressedCapacity);
		int compressedSize = LZ4_compress_default(reinterpret_cast<const char*>(data), compressedData.data(), dataSize, compressedCapacity);

		if (compressedSize <= 0) {
			std::println("LZ4 compression failed for: {}", path);
			stbi_image_free(data);
			return;
		}

		TextureHader header{ .width = static_cast<uint32_t>(width),
			                 .height = static_cast<uint32_t>(height),
			                 .channels = static_cast<uint32_t>(channels),
			                 .compressedSize = static_cast<uint32_t>(compressedSize),
			                 .uncompressedSize = dataSize };

		std::filesystem::path outputPath(path);
		outputPath.replace_extension(".cbkt");

		std::ofstream out(outputPath, std::ios::binary);
		if (!out) {
			std::println("Failed to create output file: {}", outputPath.string());
			stbi_image_free(data);
			return;
		}

		out.write(reinterpret_cast<const char*>(&header), sizeof(header));
		out.write(reinterpret_cast<const char*>(compressedData.data()), compressedSize);

		stbi_image_free(data);
		std::println("Converted: {} -> {}", path, outputPath.string());
	}

	void TextureConverter::packMetalRough(std::string_view metalPath, std::string_view roughPath,
	                                      std::string_view outputPath) {
		int mw, mh, mc;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* metalData = stbi_load(metalPath.data(), &mw, &mh, &mc, 1);
		if (!metalData) {
			std::println("Failed to load metalness texture: {}", metalPath);
			return;
		}

		int rw, rh, rc;
		stbi_uc* roughData = stbi_load(roughPath.data(), &rw, &rh, &rc, 1);
		if (!roughData) {
			std::println("Failed to load roughness texture: {}", roughPath);
			stbi_image_free(metalData);
			return;
		}

		int width = mw, height = mh;
		if (rw != mw || rh != mh) {
			std::println("Warning: Metal ({0}x{1}) and Rough ({2}x{3}) dimensions differ, using metal size",
			             mw, mh, rw, rh);
		}

		uint32_t pixelCount = static_cast<uint32_t>(width) * height;
		uint32_t channels = 4;
		uint32_t dataSize = pixelCount * channels;
		std::vector<uint8_t> packed(dataSize);

		for (uint32_t i = 0; i < pixelCount; i++) {
			uint8_t metal = metalData[i];
			uint8_t rough = (i < static_cast<uint32_t>(rw * rh)) ? roughData[i] : 0;
			packed[i * 4 + 0] = 0;       // R = unused
			packed[i * 4 + 1] = rough;   // G = roughness
			packed[i * 4 + 2] = metal;   // B = metalness
			packed[i * 4 + 3] = 255;     // A = 1
		}

		stbi_image_free(metalData);
		stbi_image_free(roughData);

		int compressedCapacity = LZ4_compressBound(dataSize);
		std::vector<char> compressedData(compressedCapacity);
		int compressedSize = LZ4_compress_default(
			reinterpret_cast<const char*>(packed.data()), compressedData.data(),
			dataSize, compressedCapacity);

		if (compressedSize <= 0) {
			std::println("LZ4 compression failed for packed MetalRough");
			return;
		}

		TextureHader header{
			.width = static_cast<uint32_t>(width),
			.height = static_cast<uint32_t>(height),
			.channels = channels,
			.compressedSize = static_cast<uint32_t>(compressedSize),
			.uncompressedSize = dataSize
		};

		std::ofstream out(outputPath.data(), std::ios::binary);
		if (!out) {
			std::println("Failed to create output file: {}", outputPath);
			return;
		}

		out.write(reinterpret_cast<const char*>(&header), sizeof(header));
		out.write(compressedData.data(), compressedSize);

		std::println("Packed MetalRough: {} + {} -> {}", metalPath, roughPath, outputPath);
	}
} // namespace cbk::ac