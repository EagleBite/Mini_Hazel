#pragma once

#include "Core.h"

namespace Hazel
{
	// 定义一个应用程序类，这是引擎的核心部分，负责管理应用程序的生命周期
	class HAZEL_API Application
	{
	public:
		// 构造函数，用于初始化应用程序
		Application();

		// 虚析构函数，保证在继承类中析构时调用基类的析构函数
		virtual ~Application();

		// 主运行函数，应用程序的核心逻辑在此运行
		void Run();

	private:
		// 表示应用程序是否正在运行的状态变量
		bool m_Running = true;
	};

	// 由客户端（引擎用户）定义的函数，用于创建具体的应用程序实例
	// 这是一个抽象工厂函数，Hazel 引擎依赖此函数来启动用户的应用程序
	Application* CreateApplication();
}
