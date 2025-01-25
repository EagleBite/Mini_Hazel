#pragma once

#include "Hazel/Core.h"

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
 {
	// ��ʼ����־ϵͳ
	Hazel::Log::Init();
	HZ_CORE_INFO("Logger initialized");

	// ����Ӧ��ʵ��
	HZ_CORE_INFO("Creating application...");
	auto app = Hazel::CreateApplication();

	// ����Ӧ��
	HZ_CORE_INFO("Running application...");
	app->Run();

	// ɾ��Ӧ��ʵ��
	HZ_CORE_INFO("Deleting application...");
	delete app;

	HZ_CORE_INFO("Application terminated");

	return 0;
}

#endif