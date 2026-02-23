#pragma once
#include "Event.hpp"

namespace MultiStation {

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const float x, const float y) noexcept 
			: m_MouseX(x), m_MouseY(y) {
		}

		float GetX() const noexcept { return m_MouseX; }
		float GetY() const noexcept { return m_MouseY; }

		std::string ToString() const noexcept override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset) noexcept
			: m_XOffset(xOffset), m_YOffset(yOffset) {
		}

		float GetXOffset() const noexcept { return m_XOffset; }
		float GetYOffset() const noexcept { return m_YOffset; }

		std::string ToString() const noexcept override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_XOffset, m_YOffset;
	};

	class MouseButtonEvent : public Event
	{
	public:
		int GetMouseButton() const noexcept { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
	protected:
		MouseButtonEvent(const int button) noexcept
			: m_Button(button) {
		}

		int m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const int button) noexcept
			: MouseButtonEvent(button) {
		}

		std::string ToString() const noexcept override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const int button) noexcept 
			: MouseButtonEvent(button) {
		}

		std::string ToString() const noexcept override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}