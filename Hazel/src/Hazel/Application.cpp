#include "hzpch.h"
#include "Application.h"
#include "Hazel/Log.h"
#include <GLFW/glfw3.h>
namespace Hazel {
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	Application::Application()
	{
		// 1.1Application创建窗口
		m_Window = std::unique_ptr<Window>(Window::Create());
		// 1.2Application设置窗口事件的回调函数
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}
	// 回调glfw窗口事件的函数
	void Application::OnEvent(Event& e)
	{
		// 4.用事件调度器，拦截自己层想要拦截的事件并处理
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		HZ_CORE_TRACE("{0}", e);
	}
	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();	// 更新glfw
		}
	}
	// 5.执行Application的OnWinClose函数拦截处理event事件
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	Application::~Application() {}
}