#pragma once

#include <functional>
#include "glm/glm.hpp"
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
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default; // 复制构造函数
		TransformComponent(const glm::mat4& transform)           // 转换构造函数
			: Transform(transform) {}

		operator const glm::mat4& () { return Transform; }       // 类型转换构造函数
		operator const glm::mat4& () const { return Transform; }
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