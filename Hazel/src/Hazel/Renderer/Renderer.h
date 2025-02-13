#pragma once

#include "RendererAPI.h"
#include "Shader.h"
#include "VertexArray.h"
#include "OrthoGraphicsCamera.h"
#include "PerspectiveGraphicsCamera.h"

namespace Hazel
{

	class Renderer
	{
	public:
		static void Init();
		static void BeginScene(OrthoGraphicsCamera& camera);
		static void EndScene();
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, glm::mat4 transform = glm::mat4(1.0f));
		static void OnWindowResize(const uint32_t width, const uint32_t height);

		inline static void SetAPI(RendererAPI::API api) { RendererAPI::SetAPI(api); }
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* m_SceneData;
	};

} // namespace Hazel