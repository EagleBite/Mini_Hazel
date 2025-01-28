#include "SandBox2D.h"
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

SandBox2D::SandBox2D()
	:Layer("SandBox2D Layer"), m_CameraController(1280.f / 720.f)
{
}

void SandBox2D::OnAttach()
{
	Hazel::Renderer2D::Init();
	m_Texture = Hazel::Texture2D::Create("assets/textures/directions.png");
}

void SandBox2D::OnDetach()
{

}

void SandBox2D::OnUpdate(Hazel::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Hazel::RenderCommand::Clear();

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.8f, 0.8f }, m_Color);
	Hazel::Renderer2D::DrawQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, m_Texture);
	Hazel::Renderer2D::EndScene();
}

void SandBox2D::OnEvent(Hazel::Event& event)
{
	m_CameraController.OnEvent(event);
}

void SandBox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("m_Color", glm::value_ptr(m_Color));
	ImGui::End();
}
