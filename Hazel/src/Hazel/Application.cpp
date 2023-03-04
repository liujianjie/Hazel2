#include "hzpch.h"
#include "Application.h"
#include "Hazel/Log.h"
#include <glad/glad.h>
namespace Hazel {
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	Application::Application()
	{
		// 1.1Application��������
		m_Window = std::unique_ptr<Window>(Window::Create());
		// 1.2Application���ô����¼��Ļص�����
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	// �ص�glfw�����¼��ĺ���
	void Application::OnEvent(Event& e)
	{
		// 4.���¼��������������Լ�����Ҫ���ص��¼�������
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		// �Ӻ���ǰ˳�����¼�
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)// ������Ͳ�Ҫ����ǰһ����
				break;
		}
	}
	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			// ��ǰ����˳����²�
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Window->OnUpdate();	// ����glfw
		}
	}
	// 5.ִ��Application��OnWinClose�������ش���event�¼�
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	Application::~Application() {}
}