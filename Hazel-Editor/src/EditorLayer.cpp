#include "EditorLayer.h"
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{
	// 场景摄像机控制器
	class CameraController : public ScriptableEntity
	{
	public:
		virtual void OnCreate() override {};
		virtual void OnDestroy() override {};
		virtual void OnUpdate(Timestep ts) override
		{
			auto &transform = GetComponent<TransformComponent>().Transform;
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
		Renderer2D::Init(); // 渲染器初始化

		// 创建帧缓冲 -- 存储渲染结果
		FrameBufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(spec);

		m_ActiveScene = CreateRef<Scene>();
		m_SceneHierarchyPanel = CreateRef<SceneHierarchyPanel>(m_ActiveScene);

		m_SquareEntity = m_ActiveScene->CreateEntity("Square Entity");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
		m_CameraEntity.AddComponent<CameraComponent>();
		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		if (m_ViewportFocused)
		{
			m_CameraController.OnUpdate(ts);
		}

		// ViewPort大小变化调整
		FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		Renderer2D::ResetStats(); // 重置渲染数据

		m_FrameBuffer->Bind();

		RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
		RenderCommand::Clear();

		m_ActiveScene->OnUpdate(ts); // 更新场景Scene

		m_FrameBuffer->UnBind();
	}

	void EditorLayer::OnEvent(Event &event)
	{
		m_CameraController.OnEvent(event);
	}

	void EditorLayer::OnImGuiRender()
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
			const ImGuiViewport *viewport = ImGui::GetMainViewport();
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
		ImGuiIO &io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		// 创建菜单栏
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("选项"))
			{
				ImGui::MenuItem("全屏", NULL, &opt_fullscreen);
				ImGui::MenuItem("内边距", NULL, &opt_padding);
				ImGui::Separator();
				if (ImGui::MenuItem("关闭", NULL, false))
					dockspaceOpen = false;
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}


		ImGui::End();

		// 设置窗口内容
		ImGui::Begin("设置");

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D 数据");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		if (m_SquareEntity)
		{
			ImGui::Separator();
			auto &squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
			ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
			ImGui::Separator();
		}

		// 矩阵的第四列
		auto &cameraTransform = m_CameraEntity.GetComponent<TransformComponent>().Transform[3];
		ImGui::DragFloat3("Camera Transform", glm::value_ptr(cameraTransform));

		ImGui::End();

		ImGui::Begin("游戏场景");

		m_ViewportFocused = ImGui::IsWindowFocused(); // 鼠标聚焦
		m_ViewportHovered = ImGui::IsWindowHovered(); // 鼠标悬停
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2 *)&viewportPanelSize)) // 改变了窗口大小
		{
			m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
		}
		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void *)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();

		m_SceneHierarchyPanel->OnImGuiRender();
	}
}
