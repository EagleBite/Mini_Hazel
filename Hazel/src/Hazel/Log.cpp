#include "pch.h"
#include "Hazel/Log.h"

namespace Hazel
{

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	// 初始化日志系统
	void Log::Init()
	{
		// 设置日志输出格式
		/*
		* 格式说明:
		*	- %^[...]%$ : 其间的内容会根据日志等级显示为彩色
		*	- [%T]      : 当前时间，格式为 HH:MM:SS
		*	- [%l]      : 日志等级：trace、debug、info、warn、error、critical
		*	- %n        : 日志器的名称
		*	- [%@]      : 文件名和行号，格式为 filename:line_number
		*	- %v        : 日志消息内容
		* 输出示例：
		*	[14:25:01] [info] HAZEL [main.cpp:21]: Core logger initialized
	    *	[14:25:01] [info] HAZEL [main.cpp:16]: This is an info message
		*	[14:25:01] [warn] HAZEL [main.cpp:17]: This is a warning
		*	[14:25:01] [error] HAZEL [main.cpp:18]: This is an error
		*/
		spdlog::set_pattern("%^[%T] [%l] %n [%@]: %v%$");

		// 创建核心日志器（Hazel 引擎内部使用）
		s_CoreLogger = spdlog::stdout_color_mt("HAZEL");
		s_CoreLogger->set_level(spdlog::level::trace); // 设置日志等级

		// 创建客户端日志器（用户层使用）
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}