#pragma once
#include "../Core.hpp"
#define BIND_EVENT_FN(x) std::bind(&x , this , std::placeholders::_1)
/**
 * @author Dimitris Smyrnakis
 * @file Event.hpp
 * @brief This is in basic state yet , but in the future can be expanded 
 * to even more event types from input socket messages to game controller's to even 
 * output socket message's . 
 */
namespace MultiStation
{
	/**
	 * @enum EventType
	 * @brief The Type of event that is generated specificly . 
	 */
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};
	
	

	/**
	 * @enum EventCategory
	 * @brief A bit flag that gives from where the event happened,
	 * is it from application , keyboard , Mouse , Mouse Button , is it
	 * even input .  
	 */
	enum EventCategory : uint32_t
	{
		None = 0,
		EventCategoryApplication	= 0x00000001,
		EventCategoryKeyboard		= 0x00000002,
		EventCategoryInput			= 0x00000004,
		EventCategoryMouse			= 0x00000008,
		EventCategoryMouseButton	= 0x00000010
	};

	// common functionality
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() noexcept { return EventType::type; }\
								virtual EventType GetEventType() const noexcept override { return GetStaticType(); }\
								virtual const char* GetName() const noexcept override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual uint32_t GetCategoryFlags() const noexcept override { return category; }

	/**
	 * @class Event
	 * @brief A General Event class that gives the information about the 
	 * event type and category that belongs . 
	 */
	class Event
	{
		friend class EventDispatcher;
	public:
		/**
		 * @brief Returns the event type
		 * @return Event type
		 */
		virtual EventType GetEventType(void)	const noexcept = 0;
		/**
		 * @brief Returns the event name
		 * @return const char* type with the name of the event
		 */
		virtual const char* GetName(void)		const noexcept = 0;
		/**
		 * @brief Returns the event category flags from EventCategory enum defined
		 * @return uint32_t flag
		 */
		virtual uint32_t GetCategoryFlags(void)		const noexcept = 0;
		/**
		 * @brief Returns the event info in string
		 * @return String
		 */
		virtual std::string ToString(void)		const noexcept { return GetName(); }
		/**
		 * @brief Checks if event is in category
		 * @return true if has these flag set , false otherwise
		 */
		bool IsInCategory(EventCategory category) noexcept 
		{
			return GetCategoryFlags() & category;
		}

	public:
		bool Handled = false;
	};

	class EventDispatcher
	{
	public:
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) noexcept 
			: m_Event(event)
		{
		}


		template<typename T >
		bool Dispatch(EventFn<T> func) noexcept 
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	};


}