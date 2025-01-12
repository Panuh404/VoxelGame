#include "GamePCH.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::s_Logger;

void Log::Init()
{
	// Set Format of the Logging
	spdlog::set_pattern("%^[%T] %n::%v%$");

	// Logger Parameters
	s_Logger = spdlog::stdout_color_mt("LOG");
	s_Logger->set_level(spdlog::level::trace);
}
