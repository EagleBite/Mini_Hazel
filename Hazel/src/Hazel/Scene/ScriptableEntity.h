#pragma once

#include "Entity.h"

namespace Hazel
{
	// 可以执行脚本的实体
	class ScriptableEntity
	{
		friend class Scene;
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	private:
		Entity m_Entity;
	};
}