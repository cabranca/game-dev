#include <algorithm>
#include <filesystem>
#include <print>
#include <string>

#include "ModelConverter.h"
#include "TextureConverter.h"

int main(int argc, char** argv) {

	if (argc < 2) {
		std::println("Usage: CBKAssetConverter <file>");
		return -1;
	}

	std::string ext = std::filesystem::path(argv[1]).extension().string();
	std::ranges::transform(ext, ext.begin(), ::tolower);

	if (ext == ".obj" || ext == ".fbx" || ext == ".gltf" || ext == ".dae")
		cbk::ac::ModelConverter::convert(argv[1]);
	else if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".tga" || ext == ".bmp" || ext == ".hdr")
		cbk::ac::TextureConverter::convert(argv[1]);
	else
		std::println("Unsupported file format: {}", ext);

	return 0;
}
