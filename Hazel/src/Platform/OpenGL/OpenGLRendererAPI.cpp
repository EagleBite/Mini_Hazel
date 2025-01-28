#include "pch.h"
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

namespace Hazel
{

	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND); // 开启混合
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 混合函数
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& VertexArray, const uint32_t indexCount)
	{
		// 按照顶点数组的索引缓冲进行绘制
		VertexArray->Bind(); // 避免忘记绑定
		uint32_t count = indexCount ? VertexArray->GetIndexBuffer()->GetCount() : indexCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLRendererAPI::SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height)
	{
		glViewport(x, y, width, height);
	}

} // namespace Hazle