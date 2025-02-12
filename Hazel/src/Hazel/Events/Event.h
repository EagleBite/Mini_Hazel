#pragma once
#include "pch.h"
#include "Hazel/Core/Core.h"
#include "Hazel/Core/Log.h"
#include <string>
#include <functional>
#include <ostream>

namespace Hazel
{
	// �¼�����ö��
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseMoved, MouseButtonPressed, MouseButtonReleased, MouseScrolled
	};

	// �¼����ö�٣�����Ϊ�¼�����
	enum EventCategory
	{
		None = 0,                          // �����
		EventCategoryApplication = BIT(0), // Ӧ���¼����
		EventCategoryInput       = BIT(1), // �����¼����
		EventCategoryKeyboard    = BIT(2), // �����¼����
		EventCategoryMouse       = BIT(3), // ����¼����
		EventCategoryMouseButton = BIT(4)  // ��갴���¼����
	};

	// �궨��:��д���麯��
	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
	#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }


	// �¼�����
	class HAZEL_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;  // ��ȡ�¼������ͣ������¼������ʵ���������
		virtual int GetCategoryFlags() const = 0;    // ��ȡ�¼�������־�������¼������ʵ���������
		virtual const char* GetName() const = 0;     // ��ȡ�¼������ƣ������¼������ʵ���������
		virtual std::string ToString() const { return GetName(); }  // �����¼����ַ�����ʾ��Ĭ�Ϸ����¼����ƣ�
		inline bool IsHandled() const { return m_Handled; }
		inline void SetHandled(const bool handled) { m_Handled |= handled; }
		bool IsInCategory(EventCategory category) const {
			return GetCategoryFlags() & category;  // ע��:һ��ʱ��������ڶ�������
		};
	protected:
		bool m_Handled = false;
	};

	// �¼��������࣬����ַ��¼�
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