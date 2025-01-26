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
		static void BeginScene(OrthoGraphicsCamera& camera);
		static void EndScene();
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static void SetAPI(RendererAPI::API api) { RendererAPI::SetAPI(api); }
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* m_SceneData;
	};

} // namespace Hazel