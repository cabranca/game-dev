#include <print>

#include "TextureConverter.h"

int main(int argc, char** argv) {

	if (argc < 2) {
		std::println("User must give a directory to convert!");
		return -1;
	}

	cbk::ac::TextureConverter::convert(argv[1]);

	return 0;
}
