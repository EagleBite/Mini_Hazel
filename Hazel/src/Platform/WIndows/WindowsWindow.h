#pragma once
#include "Hazel/Window.h"
#include "Hazel/GraphicsContext.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Hazel
{

	class HAZEL_API WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		virtual void OnUpdate() override;

		inline virtual unsigned int GetWidth() const override { return m_Data.Width; }
		inline virtual unsigned int GetHeight() const override { return m_Data.Height; }
		inline virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enable) override;
		virtual bool isVSync() const override { return m_Data.VSync; }
		inline virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		// �Զ��崰������
		struct WindowData
		{
			std::string Title;             // ���ڵı���
			unsigned int Width, Height;    // ���ڵĳ���
			bool VSync;
			EventCallbackFn EventCallback; // ���ڵĻص�����
		};

		WindowData m_Data;
	};

}