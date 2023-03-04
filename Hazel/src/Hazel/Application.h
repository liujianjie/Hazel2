#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"
#include "Hazel/Events/ApplicationEvent.h"
namespace Hazel {
	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);	// 回调窗口事件函数
	private:
		bool OnWindowClose(WindowCloseEvent& e); // 处理窗口关闭事件的函数
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};
	// To be defined in CLIENT
	Application* CreateApplication();
}