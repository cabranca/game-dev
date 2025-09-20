#pragma once

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

#include <Cabrankengine/Core/Logger.h>

namespace cabrankengine {

	class Config {
	public:
		static nlohmann::json load(const std::string& path) {
			std::ifstream file(path);
			if (!file.is_open()) {
				nlohmann::json defaults = {
					{"window", {
						{"title", "Cabrankengine"},
						{"width", 1600},
						{"height", 900}
					}}
				};

				std::ofstream out(path);
				if (out.is_open()) {
					out << defaults.dump(4);
					out.close();
					CE_CORE_WARN("[Config] File not found, creating new one in {0}", path);
				}
				else {
					CE_CORE_ERROR("[Config] File not found, failed to create a new on in {0}", path);
				}

				return defaults;
			}

			nlohmann::json j;
			try {
				file >> j;
			}
			catch (std::exception& e) {
				CE_CORE_ERROR("[Config] Failed reading {0} - Error message: {1}", path, e.what());
				return {
					{"window", {{"title", "Cabrankengine"}, {"width", 1600}, {"height", 900}}}
				};
			}
			return j;
		}
	};
}
