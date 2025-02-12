#pragma once
#include "pch.h"
#include "Hazel/Core/Core.h"
#include "Hazel/Core/Log.h"
#include <string>
#include <functional>
#include <ostream>

namespace Hazel
{
	// 事件类型枚举
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseMoved, MouseButtonPressed, MouseButtonReleased, MouseScrolled
	};

	// 事件类别枚举，用于为事件分类
	enum EventCategory
	{
		None = 0,                          // 无类别
		EventCategoryApplication = BIT(0), // 应用事件类别
		EventCategoryInput       = BIT(1), // 输入事件类别
		EventCategoryKeyboard    = BIT(2), // 键盘事件类别
		EventCategoryMouse       = BIT(3), // 鼠标事件类别
		EventCategoryMouseButton = BIT(4)  // 鼠标按键事件类别
	};

	// 宏定义:重写纯虚函数
	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
	#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }


	// 事件基类
	class HAZEL_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;  // 获取事件的类型，所有事件类必须实现这个方法
		virtual int GetCategoryFlags() const = 0;    // 获取事件的类别标志，所有事件类必须实现这个方法
		virtual const char* GetName() const = 0;     // 获取事件的名称，所有事件类必须实现这个方法
		virtual std::string ToString() const { return GetName(); }  // 返回事件的字符串表示（默认返回事件名称）
		inline bool IsHandled() const { return m_Handled; }
		inline void SetHandled(const bool handled) { m_Handled |= handled; }
		bool IsInCategory(EventCategory category) const {
			return GetCategoryFlags() & category;  // 注意:一个时间可以属于多种类型
		};
	protected:
		bool m_Handled = false;
	};

	// 事件调度器类，负责分发事件
	class HAZEL_API EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event):
			m_Event(event){}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

}