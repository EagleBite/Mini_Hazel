#pragma once
#include "Hazel/Core/Core.h"
#include "Hazel/Renderer/RendererAPI.h"

namespace Hazel
{
	// äÖÈ¾ÃüÁî -> µ÷ÓÃäÖÈ¾API
	class RenderCommand
	{
	public:
		inline static void Init(){
			s_RendererAPI->Init();
		}
		inline static void SetClearColor(const glm::vec4& color) {
			s_RendererAPI->SetClearColor(color);
		}
		inline static void Clear() {
			s_RendererAPI->Clear();
		}
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, const uint32_t indexCount = 0) {
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}
		inline static void SetViewport(const uint32_t width, const uint32_t height) {
			s_RendererAPI->SetViewport(0, 0, width, height);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};

} // namespace Hazel