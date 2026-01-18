#pragma once

// Comment out to disable console
#define ALLOW_CONSOLE

#ifdef ALLOW_CONSOLE
#define StartConsole()																									\
		AllocConsole();																									\
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);																\
		MoveWindow(GetConsoleWindow(), CONSOLE_STARTING_X, CONSOLE_STARTING_Y, CONSOLE_WIDTH, CONSOLE_HEIGHT, TRUE);
#else
#define StartConsole()
#endif

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
#ifdef ALLOW_CONSOLE
	#define PRINT_WHITE(...) SPDLOG_LOGGER_TRACE(::Log::GetLogger(), __VA_ARGS__)	// White
	#define PRINT_GREEN(...) SPDLOG_LOGGER_INFO(::Log::GetLogger(), __VA_ARGS__)	// Green
	#define PRINT_YELLOW(...) SPDLOG_LOGGER_WARN(::Log::GetLogger(), __VA_ARGS__)	// Yellow
	#define PRINT_RED(...) SPDLOG_LOGGER_ERROR(::Log::GetLogger(), __VA_ARGS__)		// Red
#else
	#define PRINT_WHITE(...)
	#define PRINT_GREEN(...)
	#define PRINT_YELLOW(...) 
	#define PRINT_RED(...)
#endif
