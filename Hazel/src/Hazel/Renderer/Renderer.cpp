#include "pch.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/RenderCommand.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/glm.hpp>

namespace Hazel
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(OrthoGraphicsCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, glm::mat4 transform /*= glm::mat4(1.0f)*/)
	{
		shader->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::OnWindowResize(const uint32_t width, const uint32_t height)
	{
		RenderCommand::SetViewport(width, height);
	}

}