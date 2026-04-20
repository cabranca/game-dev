#include <algorithm>
#include <filesystem>
#include <string>

#include <Common/Logger.h>

#include "ModelConverter.h"
#include "TextureConverter.h"

int main(int argc, char** argv) {
	cbk::common::Logger::init();

	if (argc < 2) {
		CBK_AC_ERROR("Usage: CBKAssetConverter <file>");
		return -1;
	}

	std::string ext = std::filesystem::path(argv[1]).extension().string();
	std::ranges::transform(ext, ext.begin(), ::tolower);

	if (ext == ".obj" || ext == ".fbx" || ext == ".gltf" || ext == ".dae")
		cbk::ac::ModelConverter::convert(argv[1]);
	else if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".tga" || ext == ".bmp" || ext == ".hdr")
		cbk::ac::TextureConverter::convert(argv[1]);
	else
		CBK_AC_ERROR("Unsupported file format: {}", ext);

	return 0;
}
