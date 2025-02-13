#include "EditorLayer.h"
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{
	class CameraController : public ScriptableEntity
	{
	public:
		void OnCreate() {};
		void OnDestroy() {};
		void OnUpdate(Timestep ts)
		{
			auto& transform = GetComponent<TransformComponent>().Transform;
			float speed = 5.0f;

			if (Input::IsKeyPressed(HZ_KEY_A))
				transform[3][0] -= speed * ts;
			if (Input::IsKeyPressed(HZ_KEY_D))
				transform[3][0] += speed * ts;
			if (Input::IsKeyPressed(HZ_KEY_W))
				transform[3][1] += speed * ts;
			if (Input::IsKeyPressed(HZ_KEY_S))
				transform[3][1] -= speed * ts;
		}
	};

	EditorLayer::EditorLayer() :
		Layer("EditorLayer"),
		m_CameraController(1280.f / 720.f)
	{
	}

	void EditorLayer::OnAttach()
	{
		Renderer2D::Init();

		// ֡����
		FrameBufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(spec);

		m_ActiveScene = CreateRef<Scene>();

		m_SquareEntity = m_ActiveScene->CreateEntity();
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		m_CameraEntity = m_ActiveScene->CreateEntity();
		m_CameraEntity.AddComponent<CameraComponent>();
		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		Renderer2D::ResetStats(); // ������Ⱦ����

		m_FrameBuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		// ���³���Scene
		m_ActiveScene->OnUpdate(ts);

		m_FrameBuffer->UnBind();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_CameraController.OnEvent(event);
	}

	void EditorLayer::OnImGuiRender()
	{
		// ���� ImGui ���ڱ�־������ Docking
		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			// ��ȡ�����ڴ�С
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

		// ���� DockSpace ��Ҫ�Ĵ���
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// ���� DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		// �����˵���
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

		// ���ô�������
		ImGui::Begin("Settings");
		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Statistics");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		if (m_SquareEntity)
		{
			ImGui::Separator();
			auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
			ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
			ImGui::Separator();
		}

		// ����ĵ�����
		auto& cameraTransform = m_CameraEntity.GetComponent<TransformComponent>().Transform[3];
		ImGui::DragFloat3("Camera Transform", glm::value_ptr(cameraTransform));

		ImGui::End();


		ImGui::Begin("ViewPort");

		m_ViewportFocused = ImGui::IsWindowFocused(); // ���۽�
		m_ViewportHovered = ImGui::IsWindowHovered(); // �����ͣ
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize)) // �ı��˴��ڴ�С
		{ 
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}
		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
	}
}
