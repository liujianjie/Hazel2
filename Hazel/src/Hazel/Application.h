#pragma once

#include "Core.h"
#include "Window.h"
#include "Hazel/LayerStack.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"

namespace Hazel {
	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);			// 回调窗口事件函数
		void PushLayer(Layer* layer);	// 添加层
		void PushOverlay(Layer* layer); // 添加覆盖层
	private:
		bool OnWindowClose(WindowCloseEvent& e); // 处理窗口关闭事件的函数
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};
	// To be defined in CLIENT
	Application* CreateApplication();
}