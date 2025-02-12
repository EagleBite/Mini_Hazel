#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Window.h"
#include "Hazel/Core/LayerStack.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/ImGui/ImGuiLayer.h"

namespace Hazel
{
	// ����һ��Ӧ�ó����࣬��������ĺ��Ĳ��֣��������Ӧ�ó������������
	class Application
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
		inline ImGuiLayer* GetImGuiLayer() const { return m_ImGuiLayer; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		bool m_Running = true;
		bool m_Minimized = false;
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
