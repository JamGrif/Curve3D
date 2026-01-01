#pragma once

#include "spdlog/spdlog.h"

/// <summary>
/// Log messages to the console
/// </summary>
class Log
{
public:
	static void Init();

	static std::shared_ptr<spdlog::logger>& GetLogger() { return s_pLogger; }

private:

	static std::shared_ptr<spdlog::logger>	s_pLogger;

	Log();
};

// Logging Macros
#ifdef _DEBUG
	#define PRINT_WHITE(...) SPDLOG_LOGGER_TRACE(::Log::GetLogger(), __VA_ARGS__)	// White
	#define PRINT_GREEN(...) SPDLOG_LOGGER_INFO(::Log::GetLogger(), __VA_ARGS__)	// Green
	#define PRINT_YELLOW(...) SPDLOG_LOGGER_WARN(::Log::GetLogger(), __VA_ARGS__)	// Yellow
	#define PRINT_RED(...) SPDLOG_LOGGER_ERROR(::Log::GetLogger(), __VA_ARGS__) // Red
#else
	#define PRINT_WHITE(...)
	#define PRINT_GREEN(...)
	#define PRINT_YELLOW(...) 
	#define PRINT_RED(...)
#endif
