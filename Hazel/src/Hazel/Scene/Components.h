#pragma once

#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Hazel/Renderer/SceneCamera.h"
#include "Hazel/Scene/ScriptableEntity.h"

namespace Hazel
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}

		operator const std::string& () const { return Tag; }
		operator std::string& () { return Tag; }
	};

	// 变换组件
	struct TransformComponent 
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default; // 复制构造函数
		TransformComponent(const glm::vec3& translation)         // 转换构造函数
			: Translation(translation) {}
		// 获取转换矩阵
		glm::mat4 GetTransformMatrix() const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1,0,0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	// 精灵图组件
	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	// 摄像机组件
	struct CameraComponent
	{
		SceneCamera camera;
		bool isPrimary = true; // 是否为主相机
		bool isFixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	// 原生脚本组件(Native Script Component)
	// 该组件用于存储游戏实体(Entity)绑定的脚本类实例，并提供脚本生命周期管理。
	// 主要作用：
	// 1. 允许脚本实例(ScriptableEntity)访问所属实体的其他组件。
	// 2. 通过函数绑定机制，支持不同类型的脚本动态绑定到实体。
	// 3. 负责脚本的创建、销毁及生命周期回调(OnCreate、OnDestroy、OnUpdate)。
	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr; // 脚本实体 -- 用于获取其他组件以便操作

		// 负责创建、销毁脚本实例的函数
		std::function<void()> InstantiateFunction;
		std::function<void()> DestroyInstanceFunction;

		// 绑定脚本生命周期回调
		std::function<void(ScriptableEntity*)> OnCreateFunction;
		std::function<void(ScriptableEntity*)> OnDestroyFunction;
		std::function<void(ScriptableEntity*, Timestep)> OnUpdateFunction;

		template<typename T>
		void Bind()
		{
			InstantiateFunction = [&]() { Instance = new T(); };
			DestroyInstanceFunction = [&]() { delete (T*)Instance; Instance = nullptr; };

			OnCreateFunction = [](ScriptableEntity* instance) {((T*)instance)->OnCreate(); };
			OnDestroyFunction = [](ScriptableEntity* instance) {((T*)instance)->OnDestroy(); };
			OnUpdateFunction = [](ScriptableEntity* instance, Timestep ts) {((T*)instance)->OnUpdate(ts); };
		}
	};
}