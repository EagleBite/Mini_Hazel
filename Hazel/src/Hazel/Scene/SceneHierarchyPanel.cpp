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
		if (m_SelectionContext) { // operator uint32_t() ����const����Ȼ�������operator bool(),���ǵ���uint32_t()
			DrawComponents(m_SelectionContext);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		// ��ȡʵ��ı�ǩ�����ƣ�
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		// ���� ImGui ���ڵ��־
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
		if (m_SelectionContext == entity)  // ��ǰѡ�е�ʵ��
			flags |= ImGuiTreeNodeFlags_Selected;

		// ʹ�ñ�ǩ�����ƣ���ΪΨһ ID
		ImGuiID nodeID = ImGui::GetID(tag.c_str());
		bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(nodeID), flags, tag.c_str());

		// ���������������ѡ��ʵ��
		if (ImGui::IsItemClicked())
			m_SelectionContext = entity;

		// ������ڵ㱻չ���������ӽڵ�
		if (opened)
		{
			ImGui::TreePop();  // �������ڵ�
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		// ��ǩ���
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy(buffer, tag.c_str());
		if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
			tag = std::string(buffer);
		}

		// �任���
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