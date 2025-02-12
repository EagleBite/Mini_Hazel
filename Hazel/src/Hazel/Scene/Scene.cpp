#include "pch.h"
#include "Scene.h"
#include "Components.h"
#include "Hazel/Renderer/Renderer2D.h"
#include <glm/glm.hpp>

namespace Hazel
{
	static void DoMath(const glm::mat4& transform) {

	}
	static void OnTransformConstruct(entt::registry& registry, entt::entity entity) {

	}

	Scene::Scene()
	{
		entt::entity id = m_Registry.create();
	}

	Scene::~Scene()
	{

	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
		view.each([](auto entity, TransformComponent& transform, SpriteRendererComponent& sprite) {
			 Renderer2D::DrawQuad(transform, sprite.Color);
			});
	}

	Entity Scene::CreateEntity()
	{
		Entity entity = { m_Registry.create(),this };
		entity.AddComponent<TransformComponent>();
		return entity;
	}

}