#pragma once

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Window.h"
#include "Hazel/LayerStack.h"
#include "Hazel/ImGui/ImGuiLayer.h"

namespace Hazel
{
	// 定义一个应用程序类，这是引擎的核心部分，负责管理应用程序的生命周期
	class HAZEL_API Application
	{
	public:

		Application();
		virtual ~Application();


		void Run(); // 主运行函数，应用程序的核心逻辑在此运行
		 
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		bool m_Running = true;
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

	private: 
		static Application* s_Instance;
	};

	// 由客户端（引擎用户）定义的函数，用于创建具体的应用程序实例
	// 这是一个抽象工厂函数，Hazel 引擎依赖此函数来启动用户的应用程序
	Application* CreateApplication();
}
