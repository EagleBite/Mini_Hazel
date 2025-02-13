#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Renderer/Camera.h"
#include "Hazel/Renderer/OrthoGraphicsCamera.h"
#include "Hazel/Renderer/Texture.h"
#include "Hazel/Renderer/SubTexture2D.h"
#include "glm/glm.hpp"

namespace Hazel
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene(const OrthoGraphicsCamera& camera);
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture);

		// 渲染信息
		struct Statistics
		{
			uint32_t DrawCalls = 0; // 绘制命令调用次数
			uint32_t QuadCount = 0; // 绘制的正方形数量

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();
	private:
		static void FlushAndReset();// 内存不够为了分批渲染要做的drawcall绘制和重置
		static float GetTextureIndex(const Ref<Texture2D>& texture);
	};
}