#pragma once

#include "Hazel/Core/Core.h"
#include "Scene.h"

namespace Hazel
{
	class Scene;
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) 
		{
			HZ_CORE_ASSERT(!HasComponent<T>(), "Entity already has this component.");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component.");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent() 
		{
			HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component.");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent() 
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		Scene* GetScene() { return m_Scene; }
	private:
		// The unique identifier for the entity in the registry.
		entt::entity m_EntityHandle{ entt::null };
		// A reference to the scene this entity belongs to.
		Scene* m_Scene = nullptr;
	};
}