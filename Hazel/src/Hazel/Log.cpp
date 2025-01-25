#include "pch.h"
#include "Hazel/Log.h"

namespace Hazel
{

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	// ��ʼ����־ϵͳ
	void Log::Init()
	{
		// ������־�����ʽ
		/*
		* ��ʽ˵��:
		*	- %^[...]%$ : �������ݻ������־�ȼ���ʾΪ��ɫ
		*	- [%T]      : ��ǰʱ�䣬��ʽΪ HH:MM:SS
		*	- [%l]      : ��־�ȼ���trace��debug��info��warn��error��critical
		*	- %n        : ��־��������
		*	- [%@]      : �ļ������кţ���ʽΪ filename:line_number
		*	- %v        : ��־��Ϣ����
		* ���ʾ����
		*	[14:25:01] [info] HAZEL [main.cpp:21]: Core logger initialized
	    *	[14:25:01] [info] HAZEL [main.cpp:16]: This is an info message
		*	[14:25:01] [warn] HAZEL [main.cpp:17]: This is a warning
		*	[14:25:01] [error] HAZEL [main.cpp:18]: This is an error
		*/
		spdlog::set_pattern("%^[%T] [%l] %n [%@]: %v%$");

		// ����������־����Hazel �����ڲ�ʹ�ã�
		s_CoreLogger = spdlog::stdout_color_mt("HAZEL");
		s_CoreLogger->set_level(spdlog::level::trace); // ������־�ȼ�

		// �����ͻ�����־�����û���ʹ�ã�
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}