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

	if (ext == ".obj" || ext == ".fbx" || ext == ".gltf" || ext == ".dae")
		cbk::ac::ModelConverter::convert(argv[1]);
	else
		cbk::ac::TextureConverter::convert(argv[1]);

	return 0;
}
