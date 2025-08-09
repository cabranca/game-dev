#pragma once

#include <Cabrankengine/Core/Core.h>
#include <spdlog/spdlog.h>

namespace cabrankengine {

	class CABRANKENGINE_API Logger {

	public:
		// Sets the loggers and the patterns
		static void init();

		// Returns the logger for the engine app
		static std::shared_ptr<spdlog::logger>& getCoreLogger() { return m_CoreLogger; }

		// Returns the logger for the client (game) app
		static std::shared_ptr<spdlog::logger>& getClientLogger() { return m_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> m_CoreLogger; // Logger for the engine app
		static std::shared_ptr<spdlog::logger> m_ClientLogger; // Logger for the client (game) app
	};
}

// Core log macros
#define CE_CORE_TRACE(...) ::cabrankengine::Logger::getCoreLogger()->trace(__VA_ARGS__)
#define CE_CORE_INFO(...) ::cabrankengine::Logger::getCoreLogger()->info(__VA_ARGS__)
#define CE_CORE_WARN(...) ::cabrankengine::Logger::getCoreLogger()->warn(__VA_ARGS__)
#define CE_CORE_ERROR(...) ::cabrankengine::Logger::getCoreLogger()->error(__VA_ARGS__)
#define CE_CORE_FATAL(...) ::cabrankengine::Logger::getCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define CE_TRACE(...) ::cabrankengine::Logger::getClientLogger()->trace(__VA_ARGS__)
#define CE_INFO(...) ::cabrankengine::Logger::getClientLogger()->info(__VA_ARGS__)
#define CE_WARN(...) ::cabrankengine::Logger::getClientLogger()->warn(__VA_ARGS__)
#define CE_ERROR(...) ::cabrankengine::Logger::getClientLogger()->error(__VA_ARGS__)
#define CE_FATAL(...) ::cabrankengine::Logger::getClientLogger()->fatal(__VA_ARGS__)


