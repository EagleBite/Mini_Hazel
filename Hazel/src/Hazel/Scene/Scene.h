#pragma once

#include "entt.hpp"
#include "Hazel/Scene/Entity.h"
#include "Hazel/Core/TimeStep.h"
#include <string>

namespace Hazel
{
	class Entity;
	class Scene
	{
		friend class Entity;
		friend class SceneHierarchyPanel;
	public:
		Scene();
		~Scene();
		// ½ûÖ¹¿½±´
		Scene(const Scene&) = delete;
		Scene& operator=(const Scene&) = delete;

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		entt::registry& Reg() { return m_Registry; }

		Entity CreateEntity(const std::string& name);

		void SetViewportFocused(bool condition) { m_ViewportFocused = condition; }
		void SetViewportHovered(bool condition) { m_ViewportHovered = condition; }
		bool IsViewportFocused() const { return m_ViewportFocused; }
		bool IsViewportHovered() const { return m_ViewportHovered; }
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth;
		uint32_t m_ViewportHeight;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
	};
}