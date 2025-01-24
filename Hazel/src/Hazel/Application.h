#pragma once

#include "Core.h"

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

	private:
		// ��ʾӦ�ó����Ƿ��������е�״̬����
		bool m_Running = true;
	};

	// �ɿͻ��ˣ������û�������ĺ��������ڴ��������Ӧ�ó���ʵ��
	// ����һ�����󹤳�������Hazel ���������˺����������û���Ӧ�ó���
	Application* CreateApplication();
}
