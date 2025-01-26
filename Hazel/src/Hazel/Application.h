#pragma once

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Window.h"
#include "Hazel/LayerStack.h"
#include "Hazel/ImGui/ImGuiLayer.h"

namespace Hazel
{
	// ����һ��Ӧ�ó����࣬��������ĺ��Ĳ��֣��������Ӧ�ó������������
	class HAZEL_API Application
	{
	public:

		Application();
		virtual ~Application();


		void Run(); // �����к�����Ӧ�ó���ĺ����߼��ڴ�����
		 
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
		float m_LastFrameTime;

	private: 
		static Application* s_Instance;
	};

	// �ɿͻ��ˣ������û�������ĺ��������ڴ��������Ӧ�ó���ʵ��
	// ����һ�����󹤳�������Hazel ���������˺����������û���Ӧ�ó���
	Application* CreateApplication();
}
