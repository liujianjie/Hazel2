#pragma once
#include "hzpch.h"
#include "Hazel/Core.h"
namespace Hazel {
	/*
		Ϊ�˼�㣬�Զ����¼������������¼���û�л����¼���
		�����¼�������aһֱ���µ�һ���������������һ�²�һֱ�����
	*/
	// �¼����-һ����һ����ʶ
	enum class EventType{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};
	// �¼�����-�����һ�����࣬�����������ͬһ������
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),	// 1
		EventCategoryInput			= BIT(1),	// 2
		EventCategoryKeyboard		= BIT(2),	// 4
		EventCategoryMouse			= BIT(3),	// 8
		EventCategoryMouseButton	= BIT(4)	// 16
	};
// �궨�壺ÿ�����඼��Ҫ��д�����麯�����룬�����ú궨�������
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class HAZEL_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;		// ��ȡ���¼����ĸ�����
		virtual const char* GetName() const = 0;		// ��ȡ���¼�������c�ַ�����
		virtual int GetCategoryFlags() const = 0;		// ��ȡ���¼������ĸ�����
		virtual std::string ToString() const { return GetName(); }	// ��ȡ���¼������ƴ�c�ַ�����תΪ�ַ���

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	protected:
		bool m_Handled = false;
	};
	// �¼���������
	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;	// ����function�����ܷ�������bool��������T&�ĺ���
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}
		template<typename T>
		bool Dispatch(EventFn<T> func)				// function�������պ���ָ��
		{
			if (m_Event.GetEventType() == T::GetStaticType())	// ���ص��¼����봦�����¼������Ƿ�ƥ��
			{
				m_Event.m_Handled = func(*(T*)&m_Event);		// �������ص��¼�
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;								// ���ص��¼�
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}