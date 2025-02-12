#include "SandBox2D.h"
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

SandBox2D::SandBox2D() : 
	Layer("SandBox2D Layer"), 
	m_CameraController(1280.f / 720.f),
	m_ParticleSystem(1000)
{
	// 粒子系统参数
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
	m_Texture = Hazel::Texture2D::Create("./assets/textures/directions.png");
	// 精灵图SpriteSheet
	m_SpriteSheet = Hazel::Texture2D::Create("./assets/textures/SpriteSheet.png");
	m_TextureStair = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 6 }, { 128, 128 });
	m_TextureBush = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 3 }, { 128, 128 });
	m_TextureTree = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 4, 1 }, { 128, 128 }, { 1,2 });
	// 帧缓冲
	Hazel::FrameBufferSpecification spec;
	spec.Width = 1280;
	spec.Height = 720;
	m_FrameBuffer = Hazel::FrameBuffer::Create(spec);
}

void SandBox2D::OnDetach()
{

}

void SandBox2D::OnUpdate(Hazel::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Hazel::Renderer2D::ResetStats();

	m_FrameBuffer->Bind();

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
				Hazel::Renderer2D::DrawQuad(position, { 0.8f, 0.8f }, m_TextureTree);
			}
			else {
				Hazel::Renderer2D::DrawQuad(position, { 0.8f, 0.8f }, m_Texture);
			}
		}
	}
	Hazel::Renderer2D::EndScene();

	m_FrameBuffer->UnBind();
}

void SandBox2D::OnEvent(Hazel::Event& event)
{
	m_CameraController.OnEvent(event);
}

void SandBox2D::OnImGuiRender()
{
	// 启用 ImGui 窗口标志，允许 Docking
	static bool dockspaceOpen = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		// 获取主窗口大小
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	// 创建 DockSpace 需要的窗口
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// 创建 DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	// 创建菜单栏
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			ImGui::MenuItem("Full Screen", NULL, &opt_fullscreen);
			ImGui::MenuItem("Padding", NULL, &opt_padding);
			ImGui::Separator();
			if (ImGui::MenuItem("Close", NULL, false))
				dockspaceOpen = false;
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::End();

	// 设置窗口内容
	ImGui::Begin("Settings");
	auto stats = Hazel::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Statistics");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("m_Color", glm::value_ptr(m_Color));

	uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
	ImGui::Image((void*)textureID, ImVec2(1280, 720), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}
