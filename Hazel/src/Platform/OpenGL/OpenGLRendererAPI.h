#pragma once
#include "Hazel/Renderer/RendererAPI.h"
#include "Hazel/Renderer/VertexArray.h"

namespace Hazel
{

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, const uint32_t indexCount = 0) override;
		virtual void SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) override;
	};

} // namespace Hazel