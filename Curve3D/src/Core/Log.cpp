#include "pch.h"

#include "spdlog/logger.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#ifdef ALLOW_CONSOLE
#include <windows.h>
#endif

std::shared_ptr<spdlog::logger> Log::s_pLogger;

namespace
{
	constexpr int CONSOLE_STARTING_X = -900;
	constexpr int CONSOLE_STARTING_Y = 525;

	constexpr int CONSOLE_WIDTH = 900;
	constexpr int CONSOLE_HEIGHT = 500;
}

/// <summary>
/// Create logger and set format of log message
/// </summary>
void Log::Init()
{
	StartConsole();

	s_pLogger = spdlog::stdout_color_mt("Curve3D");

	s_pLogger->set_level(spdlog::level::trace);

	//%n logger name, %^ colouring, %s source file, %v%$ actual log message
	spdlog::set_pattern("%n: %^ %s %v%$");
}
