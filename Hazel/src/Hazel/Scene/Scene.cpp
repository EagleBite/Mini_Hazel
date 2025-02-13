#include "pch.h"
#include "Scene.h"
#include "Components.h"
#include "Hazel/Core/Log.h"
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
		// 寻找所有用于脚本组件的实体
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
			if (!nsc.Instance) {
				nsc.InstantiateFunction();
				nsc.Instance->m_Entity = Entity{ entity, this };
				nsc.OnCreateFunction(nsc.Instance);
			}
			nsc.OnUpdateFunction(nsc.Instance, ts);
			});

		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;

		auto group = m_Registry.view<TransformComponent, CameraComponent>(); // 寻找所有拥有变换组件和摄像机组件的实体
		for (auto entity : group) 
		{
			auto& [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

			if (camera.isPrimary)
			{
				mainCamera = &camera.camera;
				cameraTransform = &transform.Transform;
				break;
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

			auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
			view.each([](auto entity, TransformComponent& transform, SpriteRendererComponent& sprite) {
				Renderer2D::DrawQuad(transform, sprite.Color);
				});

			Renderer2D::EndScene();
		}
	
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		m_Registry.view<CameraComponent>().each([=](auto entity, CameraComponent& cameraComponent) {
			if (!cameraComponent.isFixedAspectRatio) {
				cameraComponent.camera.SetViewportSize(width, height);
			}
			});
	}

	Entity Scene::CreateEntity()
	{
		Entity entity = { m_Registry.create(),this };
		entity.AddComponent<TransformComponent>();
		return entity;
	}

}