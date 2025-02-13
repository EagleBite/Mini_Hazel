#include "pch.h"
#include "SceneHierarchyPanel.h"
#include "Components.h"
#include "Hazel/Core/Log.h"
#include <imgui.h>
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
		ImGui::Begin("Scene Hierarchy");
		m_Context->m_Registry.view<TagComponent>().each([&](auto entityID, auto& tag) {
			Entity entity{ entityID, m_Context.get() };
			DrawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			m_SelectionContext = {};
		}
		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext) { // operator uint32_t() 的是const，不然不会调用operator bool(),而是调用uint32_t()
			DrawComponents(m_SelectionContext);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		// 获取实体的标签（名称）
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		// 设置 ImGui 树节点标志
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
		if (m_SelectionContext == entity)  // 当前选中的实体
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
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy(buffer, tag.c_str());
		if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
			tag = std::string(buffer);
		}

		// 变换组件
		if (entity.HasComponent<TransformComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) 
			{
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
				ImGui::TreePop();
			}
		}
	}

}