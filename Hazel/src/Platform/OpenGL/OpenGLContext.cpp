#include "pch.h"
#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel
{

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(m_WindowHandle, "Window Hadle is NULL!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

		HZ_CORE_INFO("OpenGL ��Ϣ:");
		HZ_CORE_INFO("	Vendor��{0}", (const char*)glGetString(GL_VENDOR));
		HZ_CORE_INFO("	�Կ�����{0}", (const char*)glGetString(GL_RENDERER));
		HZ_CORE_INFO("	�汾��{0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);// ��������
	}

} // namespace Hazel