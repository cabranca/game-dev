#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

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


