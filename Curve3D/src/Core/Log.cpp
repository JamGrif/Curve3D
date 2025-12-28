#include "pch.h"

#include "spdlog/logger.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::s_pLogger;

/// <summary>
/// Create logger and set format of log message
/// </summary>
void Log::Init()
{
	s_pLogger = spdlog::stdout_color_mt("Curve3D");

	s_pLogger->set_level(spdlog::level::trace);

	//%n name of the logger, %^ colouring, %s source file, %v%$ actual log message
	spdlog::set_pattern("%n: %^ %s %v%$");
}
