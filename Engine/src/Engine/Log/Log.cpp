#include "pch.h"

#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace tsEngine
{
	Ref<spdlog::logger> Log::s_Logger;

	void Log::Init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(CreateRef<spdlog::sinks::stdout_color_sink_mt>());
		//logSinks.emplace_back(CreateRef<spdlog::sinks::basic_file_sink_mt>("tsEngine.log", true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		//logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		s_Logger = CreateRef<spdlog::logger>("ENGINE", logSinks.begin(), logSinks.end());
		spdlog::register_logger(s_Logger);
		s_Logger->set_level(spdlog::level::info);
		s_Logger->flush_on(spdlog::level::info);
	}
}