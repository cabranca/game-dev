#pragma once

#include "Core.h"
#include <spdlog/spdlog.h>

namespace cabrankengine {

	class CABRANKENGINE_API Logger {

	public:
		static void init();
		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return m_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return m_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> m_CoreLogger;
		static std::shared_ptr<spdlog::logger> m_ClientLogger;
	};
}

// Core log macros
#define CE_CORE_TRACE(...) ::cabrankengine::Logger::getCoreLogger()->trace(__VA_ARGS__)
#define CE_CORE_INFO(...) ::cabrankengine::Logger::getCoreLogger()->info(__VA_ARGS__)
#define CE_CORE_WARN(...) ::cabrankengine::Logger::getCoreLogger()->warn(__VA_ARGS__)
#define CE_CORE_ERRROR(...) ::cabrankengine::Logger::getCoreLogger()->error(__VA_ARGS__)
#define CE_CORE_FATAL(...) ::cabrankengine::Logger::getCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define CE_TRACE(...) ::cabrankengine::Logger::getClientLogger()->trace(__VA_ARGS__)
#define CE_INFO(...) ::cabrankengine::Logger::getClientLogger()->info(__VA_ARGS__)
#define CE_WARN(...) ::cabrankengine::Logger::getClientLogger()->warn(__VA_ARGS__)
#define CE_ERRROR(...) ::cabrankengine::Logger::getClientLogger()->error(__VA_ARGS__)
#define CE_FATAL(...) ::cabrankengine::Logger::getClientLogger()->fatal(__VA_ARGS__)


