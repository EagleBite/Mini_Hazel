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

	// �任���
	struct TransformComponent 
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default; // ���ƹ��캯��
		TransformComponent(const glm::vec3& translation)         // ת�����캯��
			: Translation(translation) {}
		// ��ȡת������
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

	// ����ͼ���
	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	// ��������
	struct CameraComponent
	{
		SceneCamera camera;
		bool isPrimary = true; // �Ƿ�Ϊ�����
		bool isFixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	// ԭ���ű����(Native Script Component)
	// ��������ڴ洢��Ϸʵ��(Entity)�󶨵Ľű���ʵ�������ṩ�ű��������ڹ���
	// ��Ҫ���ã�
	// 1. ����ű�ʵ��(ScriptableEntity)��������ʵ������������
	// 2. ͨ�������󶨻��ƣ�֧�ֲ�ͬ���͵Ľű���̬�󶨵�ʵ�塣
	// 3. ����ű��Ĵ��������ټ��������ڻص�(OnCreate��OnDestroy��OnUpdate)��
	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr; // �ű�ʵ�� -- ���ڻ�ȡ��������Ա����

		// ���𴴽������ٽű�ʵ���ĺ���
		std::function<void()> InstantiateFunction;
		std::function<void()> DestroyInstanceFunction;

		// �󶨽ű��������ڻص�
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