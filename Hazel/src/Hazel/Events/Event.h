#pragma once
#include "hzpch.h"
#include "Hazel/Core.h"
namespace Hazel {
	/*
		为了简便，自定义事件是立即处理事件，没有缓冲事件。
		缓冲事件：键盘a一直按下第一个立刻输出，顿了一下才一直输出。
	*/
	// 事件类别-一个类一个标识
	enum class EventType{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};
	// 事件分类-多个类一个分类，即多个类属于同一个分类
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),	// 1
		EventCategoryInput			= BIT(1),	// 2
		EventCategoryKeyboard		= BIT(2),	// 4
		EventCategoryMouse			= BIT(3),	// 8
		EventCategoryMouseButton	= BIT(4)	// 16
	};
// 宏定义：每个子类都需要重写父类虚函数代码，可以用宏定义简洁代码
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class HAZEL_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;		// 获取本事件是哪个类型
		virtual const char* GetName() const = 0;		// 获取本事件的名称c字符数组
		virtual int GetCategoryFlags() const = 0;		// 获取本事件属于哪个分类
		virtual std::string ToString() const { return GetName(); }	// 获取本事件的名称从c字符数组转为字符串

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	protected:
		bool m_Handled = false;
	};
	// 事件调度器类
	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;	// 声明function，接受返回类型bool，参数是T&的函数
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}
		template<typename T>
		bool Dispatch(EventFn<T> func)				// function参数接收函数指针
		{
			if (m_Event.GetEventType() == T::GetStaticType())	// 拦截的事件和想处理的事件类型是否匹配
			{
				m_Event.m_Handled = func(*(T*)&m_Event);		// 处理拦截的事件
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;								// 拦截的事件
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}