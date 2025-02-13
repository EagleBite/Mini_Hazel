#pragma once

#include "entt.hpp"
#include "Hazel/Scene/Entity.h"
#include "Hazel/Core/TimeStep.h"

namespace Hazel
{
	class Entity;
	class Scene
	{
		friend class Entity; // ��Ԫ
	public:
		Scene();
		~Scene();
		// ��ֹ����
		Scene(const Scene&) = delete;
		Scene& operator=(const Scene&) = delete;

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		entt::registry& Reg() { return m_Registry; }

		Entity CreateEntity();

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth;
		uint32_t m_ViewportHeight;
	};
}