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

		entt::registry& Reg() { return m_Registry; }

		Entity CreateEntity();

	private:
		entt::registry m_Registry;
	};
}