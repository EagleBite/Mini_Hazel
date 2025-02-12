#include "SandBox2D.h"
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

SandBox2D::SandBox2D() : 
	Layer("SandBox2D Layer"), 
	m_CameraController(1280.f / 720.f),
	m_ParticleSystem(1000)
{
	m_Particle.Position = { 0.0f, 0.0f };
	m_Particle.Velocity = { 0.0f, 1.0f };
	m_Particle.VelocityVariation = { 0.5f, 0.5f };
	m_Particle.SizeBegin = 0.5f;
	m_Particle.SizeEnd = 0.1f;
	m_Particle.ColorBegin = glm::vec4(0.0f, 0.5f, 0.5f, 1.0f);
	m_Particle.ColorEnd = glm::vec4(0.0f);
	m_Particle.SizeVariation = 0.2f;
	m_Particle.LifeTime = 1.0f;
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
	Hazel::Renderer2D::ResetStats();

	m_CameraController.OnUpdate(ts);

	Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Hazel::RenderCommand::Clear();

	// 检测鼠标左键是否按下
	if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Hazel::Input::GetMousePosition();
		auto width = Hazel::Application::Get().GetWindow().GetWidth();
		auto height = Hazel::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}
	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

	float spacing = 0.1f; // 间距
	int count = 10; // 每行/列渲染的 Quad 数量（总计 100 * 100 = 10000 个）
	for (int x = 0; x < count; x++) {
		for (int y = 0; y < count; y++) {
			// 计算每个 Quad 的位置
			glm::vec2 position = { x * (0.8f + spacing), y * (0.8f + spacing) };

			if ((x + y) % 2 == 0) {
				Hazel::Renderer2D::DrawQuad(position, { 0.8f, 0.8f }, m_Color);
			}
			else {
				Hazel::Renderer2D::DrawQuad(position, { 0.8f, 0.8f }, m_Texture);
			}
		}
	}
	Hazel::Renderer2D::EndScene();
}

void SandBox2D::OnEvent(Hazel::Event& event)
{
	m_CameraController.OnEvent(event);
}

void SandBox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");

	auto stats = Hazel::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Statics");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("m_Color", glm::value_ptr(m_Color));
	ImGui::End();
}
