#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Renderer/OrthoGraphicsCamera.h"
#include "Hazel/Renderer/Texture.h"
#include "glm/glm.hpp"

namespace Hazel
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene(const OrthoGraphicsCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
	};
}