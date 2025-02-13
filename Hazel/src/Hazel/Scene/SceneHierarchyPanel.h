#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Scene/Scene.h"
#include "Hazel/Scene/Entity.h"
#include <glm/glm.hpp>

namespace Hazel
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
		void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float tagColumnWidth = 125.0f);

	private:
		Ref<Scene> m_Context;      // 对应场景
		Entity m_SelectionContext; // 当前被选中的实体
	};
}