#pragma once

#include "Hazel/Core.h"

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
 {
	// 初始化日志系统
	Hazel::Log::Init();
	HZ_CORE_INFO("Logger initialized");

	// 创建应用实例
	HZ_CORE_INFO("Creating application...");
	auto app = Hazel::CreateApplication();

	// 运行应用
	HZ_CORE_INFO("Running application...");
	app->Run();

	// 删除应用实例
	HZ_CORE_INFO("Deleting application...");
	delete app;

	HZ_CORE_INFO("Application terminated");

	return 0;
}

#endif