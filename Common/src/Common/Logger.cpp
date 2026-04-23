#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace cbk::common {

	std::shared_ptr<spdlog::logger> Logger::m_CoreLogger;
	std::shared_ptr<spdlog::logger> Logger::m_ClientLogger;
	std::shared_ptr<spdlog::logger> Logger::m_AssetConverterLogger;

	
	void Logger::init() {
		spdlog::set_pattern("´%^[%T] %n: %v%$");

		m_CoreLogger = spdlog::stdout_color_mt("CBK");
		m_CoreLogger->set_level(spdlog::level::trace);

		m_ClientLogger = spdlog::stdout_color_mt("APP");
		m_ClientLogger->set_level(spdlog::level::trace);

		m_AssetConverterLogger = spdlog::stdout_color_mt("ASSET CONVERTER");
		m_AssetConverterLogger->set_level(spdlog::level::trace);
	}

} // namespace cbk::common
