#pragma once
#include "Event.hpp"

namespace MultiStation {

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height) noexcept
			: m_Width(width), m_Height(height) {
		}

		uint32_t GetWidth() const noexcept { return m_Width; }
		uint32_t GetHeight() const noexcept { return m_Height; }

		std::string ToString() const noexcept override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		uint32_t m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent(void) noexcept = default;

		EVENT_CLASS_TYPE(WindowClose)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent(bool hasFocus) noexcept {
			m_focus = hasFocus;
		}
		bool IsFocus(void) noexcept {
			return m_focus;
		};
		EVENT_CLASS_TYPE(WindowFocus)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		bool m_focus;
	};

};
