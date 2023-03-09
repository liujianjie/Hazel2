#include "hzpch.h"
#include "Application.h"
#include "Hazel/Log.h"
#include <glfw/glfw3.h>
namespace Hazel {
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "引用已经存在");
		s_Instance = this;

		// 1.1Application创建窗口
		m_Window = std::unique_ptr<Window>(Window::Create());
		// 1.2Application设置窗口事件的回调函数
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		// 将ImGui层放在最后
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();		// Layer层附加时要执行的函数
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	// 回调glfw窗口事件的函数
	void Application::OnEvent(Event& e)
	{
		// 4.用事件调度器，拦截自己层想要拦截的事件并处理
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		// 从后往前顺序处理事件
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)// 处理完就不要传入前一个层
				break;
		}
	}
	void Application::Run()
	{
		while (m_Running)
		{
			// 计算两帧间隔时间
			float time = (float)glfwGetTime();// 是从应用开始计算总共的时间
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			// 从前往后顺序更新层
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			// 从前往后顺序更新层的ImGui
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

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