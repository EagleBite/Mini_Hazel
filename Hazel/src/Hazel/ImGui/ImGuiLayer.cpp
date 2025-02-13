#include "pch.h"
#include "Hazel/Core/Core.h"
#include "hazel/Core/Log.h"
#include "Hazel/Core/Application.h"
#include "Hazel/ImGui/ImGuiLayer.h"

#include <filesystem>
#include "imgui.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define IMGUI_IMPL_API
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace Hazel
{

	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// 加载支持中文的字体
		io.Fonts->Clear(); // 清除默认字体
		const char* fontPath = "./assets/fonts/AaGuShiHui-2.ttf";
		if (std::filesystem::exists(fontPath)) {
			HZ_CORE_INFO("Font file exists: {}", fontPath);
		}
		else {
			HZ_CORE_ERROR("Font file NOT found: {}", fontPath);
		}
		io.Fonts->AddFontDefault();
		ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath, 24.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
		if (!font) {
			HZ_CORE_ERROR("Failed to load font: {}", fontPath);
		}
		else {
			HZ_CORE_INFO("Font loaded successfully: {}", fontPath);
			io.FontDefault = font;
		}
		io.Fonts->Build(); // 构建字体纹理
		if (!io.Fonts->IsBuilt()) {
			HZ_CORE_ERROR("Font Atlas not built!");
		}
		else {
			HZ_CORE_INFO("Font Atlas built successfully!");
		} 

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{
		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);// 当前OnImGuiRender层显示DemoUI窗口
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			event.SetHandled(event.IsInCategory(EventCategoryMouse) && io.WantCaptureMouse);
			event.SetHandled(event.IsInCategory(EventCategoryKeyboard) && io.WantCaptureKeyboard);
		}
	}

}