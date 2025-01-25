#pragma once
#include "pch.h"
#include "Hazel/Events/Event.h"

namespace Hazel
{
	class HAZEL_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return this->m_keyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keycode)
			: m_keyCode(keycode) {}

		int m_keyCode;
	};

	class HAZEL_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repearCount) :
			KeyEvent(keycode), m_RepeatCount(repearCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};

	class HAZEL_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode) :
			KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	};

	class HAZEL_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode) :
			KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}