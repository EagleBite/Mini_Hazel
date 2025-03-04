#pragma once

#include "Entity.h"

namespace Hazel
{
	// 可以执行脚本的实体
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