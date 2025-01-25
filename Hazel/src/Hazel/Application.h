#pragma once

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Window.h"
#include "Hazel/Layer.h"
#include "Hazel/LayerStack.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"

namespace Hazel
{
	// ����һ��Ӧ�ó����࣬��������ĺ��Ĳ��֣��������Ӧ�ó������������
	class HAZEL_API Application
	{
	public:
		// ���캯�������ڳ�ʼ��Ӧ�ó���
		Application();

		// ��������������֤�ڼ̳���������ʱ���û������������
		virtual ~Application();

		// �����к�����Ӧ�ó���ĺ����߼��ڴ�����
		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		// ��ʾӦ�ó����Ƿ��������е�״̬����
		bool m_Running = true;
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

	private:
		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<Shader> m_Shader;

	private: 
		static Application* s_Instance;
	};

	// �ɿͻ��ˣ������û�������ĺ��������ڴ��������Ӧ�ó���ʵ��
	// ����һ�����󹤳�������Hazel ���������˺����������û���Ӧ�ó���
	Application* CreateApplication();
}
