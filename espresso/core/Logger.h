#pragma once
#include "config.h"
#include <memory>

#define SPDLOG_COMPILED_LIB 1
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace Espresso::Internals
{
	inline std::shared_ptr<spdlog::logger> coreLogger;
	inline std::shared_ptr<spdlog::logger> appLogger;

	inline void initLogger()
	{
		coreLogger = spdlog::stdout_color_mt("Espresso");
		appLogger = spdlog::stdout_color_mt("App");

		coreLogger->set_level(spdlog::level::trace);
		appLogger->set_level(spdlog::level::trace);

		spdlog::set_default_logger(appLogger);

		coreLogger->set_pattern("[%^%n%$][%T][%l (%t)] %v");
		appLogger->set_pattern("[%^%n%$][%T][%l][%s:%# %!()] %v");
	}
}

#define es_info(msg, ...) SPDLOG_INFO(msg, ##__VA_ARGS__)
#define es_warn(msg, ...) SPDLOG_WARN(msg, ##__VA_ARGS__)
#define es_error(msg, ...) SPDLOG_ERROR(msg, ##__VA_ARGS__)
#define es_fatal(msg, ...) SPDLOG_CRITICAL(msg, ##__VA_ARGS__)
#define es_trace(msg, ...) SPDLOG_TRACE(msg, ##__VA_ARGS__)

#ifdef DEBUG
#	define es_debug(msg, ...) SPDLOG_DEBUG(msg, ##__VA_ARGS__)
#	define es_assert(condition, msg)                                                    \
		{                                                                                \
			if (!static_cast<bool>(condition))                                           \
				SPDLOG_ERROR("Assertion \"{}\" failed: {}", #condition, msg);            \
		}
#else
#	define es_debug(msg, ...)
#	define es_assert(condition, msg)
#endif

#define es_coreInfo(msg, ...) Espresso::Internals::coreLogger->info(msg, ##__VA_ARGS__)
#define es_coreWarn(msg, ...) Espresso::Internals::coreLogger->warn(msg, ##__VA_ARGS__)
#define es_coreError(msg, ...) Espresso::Internals::coreLogger->error(msg, ##__VA_ARGS__)
#define es_coreFatal(msg, ...)                                                           \
	Espresso::Internals::coreLogger->critical(msg, ##__VA_ARGS__)
#define es_coreTrace(msg, ...) Espresso::Internals::coreLogger->trace(msg, ##__VA_ARGS__)

#ifdef DEBUG
#	define es_coreDebug(msg, ...)                                                       \
		Espresso::Internals::coreLogger->debug(msg, ##__VA_ARGS__)
#	define es_coreAssert(condition, msg)                                                \
		{                                                                                \
			if (!static_cast<bool>(condition))                                           \
				Espresso::Internals::coreLogger->error("Assertion \"{}\" failed: {}",    \
													   #condition, msg);                 \
		}
#else
#	define es_coreDebug(msg, ...)
#	define es_coreAssert(condition, msg)
#endif