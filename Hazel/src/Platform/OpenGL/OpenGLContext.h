#pragma once
#include "Hazel/Core/Core.h"
#include "Hazel/Core/Log.h"
#include "Hazel/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Hazel
{

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

} // namespace Hazel