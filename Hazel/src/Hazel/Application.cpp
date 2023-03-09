#include "hzpch.h"
#include "Application.h"
#include "Hazel/Log.h"
#include <glfw/glfw3.h>
namespace Hazel {
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "�����Ѿ�����");
		s_Instance = this;

		// 1.1Application��������
		m_Window = std::unique_ptr<Window>(Window::Create());
		// 1.2Application���ô����¼��Ļص�����
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		// ��ImGui��������
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();		// Layer�㸽��ʱҪִ�еĺ���
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
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
			// ������֡���ʱ��
			float time = (float)glfwGetTime();// �Ǵ�Ӧ�ÿ�ʼ�����ܹ���ʱ��
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			// ��ǰ����˳����²�
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			// ��ǰ����˳����²��ImGui
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

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