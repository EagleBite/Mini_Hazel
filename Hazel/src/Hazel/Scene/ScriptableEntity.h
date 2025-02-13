#pragma once

#include "Entity.h"

namespace Hazel
{
	// ����ִ�нű���ʵ��
	class ScriptableEntity
	{
		friend class Scene;
	public:
		virtual ~ScriptableEntity() {}
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
		Entity m_Entity;
	};
}