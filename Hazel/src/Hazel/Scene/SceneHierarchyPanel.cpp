#include "pch.h"
#include "SceneHierarchyPanel.h"
#include "Components.h"
#include "Hazel/Core/Log.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel
{

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("场景管理");
		m_Context->m_Registry.view<TagComponent>().each([&](auto entityID, auto& tag) {
			Entity entity{ entityID, m_Context.get() };
			DrawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			m_SelectionContext = {};
		}
		ImGui::End();

		ImGui::Begin("对象属性");
		if (m_SelectionContext) 
		{
			DrawComponents(m_SelectionContext); // 绘制实体属性面板

			ImGui::Separator();

			float buttonWidth = 150.0f; // 按钮宽度
			float availableWidth = ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2.0f;
			float offsetX = (availableWidth - buttonWidth) * 0.5f;
			ImGui::SetCursorPosX(offsetX);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);        // 圆角半径为 10.0f
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 0));
			if (ImGui::Button("+ 添加组件", ImVec2(buttonWidth, 0.0f))) {
				ImGui::OpenPopup("AddComponent");
			}
			ImGui::PopStyleVar(2);

			// 添加组件 -- 弹出窗口
			if (ImGui::BeginPopup("AddComponent")) {
				if (ImGui::MenuItem("Camera")) {
				}
				if (ImGui::MenuItem("Sprite Renderer")) {
				}
				ImGui::EndPopup();
			}

		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		// 获取实体的标签（名称）
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		// 设置 ImGui 树节点标志
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
		if (m_SelectionContext == entity)
			flags |= ImGuiTreeNodeFlags_Selected;

		// 使用标签（名称）作为唯一 ID
		ImGuiID nodeID = ImGui::GetID(tag.c_str());
		bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(nodeID), flags, tag.c_str());

		// 处理鼠标点击，更新选中实体
		if (ImGui::IsItemClicked())
			m_SelectionContext = entity;

		// 如果树节点被展开，绘制子节点
		if (opened)
		{
			ImGui::TreePop();  // 弹出父节点
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		// 标签组件
		if (entity.HasComponent<TagComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(TagComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Tag"))
			{
				auto& tag = entity.GetComponent<TagComponent>().Tag;
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy(buffer, tag.c_str());
				if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
					tag = std::string(buffer);
				}

				ImGui::TreePop(); // 确保每个 TreeNodeEx() 后都有对应的 TreePop()
			}
		}
		
		// 变换组件
		if (entity.HasComponent<TransformComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) 
			{
				auto& tfc = entity.GetComponent<TransformComponent>();
				DrawVec3Control("Translation", tfc.Translation);
				glm::vec3 rotation = glm::degrees(tfc.Rotation);
				DrawVec3Control("Rotation", glm::degrees(rotation));
				tfc.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", tfc.Scale, 1.0f);

				ImGui::TreePop(); // 确保每个 TreeNodeEx() 后都有对应的 TreePop()
			}
		}

		// 摄像机组件
		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.camera;

				ImGui::Checkbox("主摄像机", &cameraComponent.isPrimary);

				constexpr char* projectionTypeStrings[] = { "透视投影", "正交投影" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
					for (int i = 0; i < std::size(projectionTypeStrings); i++) {
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						
						// 绘制选择项
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						// 默认选中项
						if (isSelected)
							ImGui::SetItemDefaultFocus(); 
					}
					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
					float fov = glm::degrees(camera.GetPerspectiveFOV());
					float perspectiveNear = camera.GetPerspectiveNear();
					float perspectiveFar = camera.GetPerspectiveFar();
					if (ImGui::DragFloat("FOV", &fov)) {
						camera.SetPerspectiveFOV(fov);
					}
					if (ImGui::DragFloat("Near", &perspectiveNear)) {
						camera.SetPerspectiveNear(perspectiveNear);
					}
					if (ImGui::DragFloat("Far", &perspectiveFar)) {
						camera.SetPerspectiveFar(perspectiveFar);
					}
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
					float size = camera.GetOrthographicSize();
					float orthographicNear = camera.GetOrthographicNear();
					float orthographicFar = camera.GetOrthographicFar();
					if (ImGui::DragFloat("Size", &size)) {
						camera.SetOrthographicSize(size);
					}
					if (ImGui::DragFloat("Near", &orthographicNear)) {
						camera.SetOrthographicNear(orthographicNear);
					}
					if (ImGui::DragFloat("Far", &orthographicFar)) {
						camera.SetOrthographicFar(orthographicFar);
					}
				}

				ImGui::TreePop(); // 确保每个 TreeNodeEx() 后都有对应的 TreePop()
			}
		}
	}

	void SceneHierarchyPanel::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue /*= 0.0f*/, float tagColumnWidth /*= 100.0f*/)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);  // 设置为两列布局，第一列显示标签，第二列显示控件

		// 第一列：标签
		ImGui::SetColumnWidth(0, tagColumnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		// 第二列：放入 3 个控件，X, Y, Z 输入框
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 }); // 设置控件之间的间距为 0，保证按钮和输入框在同一行
		
		ImGuiContext* context = ImGui::GetCurrentContext();
		float lineHeight = context->Font->FontSize + context->Style.FramePadding.y * 2.0f; // 计算行高
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };                             // 按钮的大小，宽度比行高略宽

		// 获取第二列宽度，减去按钮的宽度，剩余的分配给 DragFloat 控件
		float secondColumnWidth = ImGui::GetColumnWidth(1);
		float remainingWidth = secondColumnWidth - (buttonSize.x * 3);  // 减去 3 个按钮的宽度
		float inputWidth = remainingWidth / 3;                          // 平均分配宽度

		// === X 轴控件 ===
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });        // 设置按钮颜色
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });  // 设置鼠标悬停时的颜色
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });  // 设置按钮按下时的颜色
		if (ImGui::Button("X", buttonSize)) {
			values.x = resetValue;  // 重置 x 分量
		}
		ImGui::PopStyleColor(3);    // 恢复按钮颜色

		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);                                     // 设置输入框宽度
		ImGui::DragFloat("##X", &values.x, 0.1f, -FLT_MAX, FLT_MAX, "%.2f");  // 绘制 X 的拖动输入框
		ImGui::PopItemWidth(); 

		// === Y 轴控件 ===
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });         // 设置按钮颜色 
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });  // 设置鼠标悬停时的颜色
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });	  // 设置按钮按下时的颜色
		if (ImGui::Button("Y", buttonSize)) { 
			values.y = resetValue;   // 重置 y 分量
		}
		ImGui::PopStyleColor(3);     // 恢复按钮颜色

		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);                              // 设置输入框宽度
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");  // 绘制 Y 的拖动输入框
		ImGui::PopItemWidth();       // 恢复默认宽度

		// === Z 轴控件 ===
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });        // 设置按钮颜色 
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f }); // 设置鼠标悬停时的颜色
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });  // 设置按钮按下时的颜色
		if (ImGui::Button("Z", buttonSize)) { 
			values.z = resetValue;  // 重置 z 分量
		}
		ImGui::PopStyleColor(3);    // 恢复按钮颜色

		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);                              // 设置输入框宽度
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");  // 绘制 Z 的拖动输入框
		ImGui::PopItemWidth();      // 恢复默认宽度

		ImGui::PopStyleVar();       // 恢复控件间距
		ImGui::Columns(1);          // 恢复为单列布局
		ImGui::PopID();             // 恢复之前的 ID 设置
	}

}