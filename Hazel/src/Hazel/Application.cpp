#include "hzpch.h"
#include "Application.h"
#include "Hazel/Log.h"
#include "Hazel/Input.h"

#include <glad/glad.h>
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

		// ʹ��OpenGL������Ⱦһ��������
		// ��������
		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};
		unsigned int indices[3] = { 0, 1, 2 }; // ��������
		// 0.���ɶ����������VAO�����㻺�����VBO�������������EBO
		glGenVertexArrays(1, &m_VertexArray);
		glGenBuffers(1, &m_VertexBuffer);
		glGenBuffers(1, &m_IndexBuffer);
		// 1. �󶨶����������
		glBindVertexArray(m_VertexArray);
		// 2. �����ǵ�CPU�Ķ������ݸ��Ƶ�GPU���㻺���У���OpenGLʹ��
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// 3. �������ǵ�CPU���������ݵ�GPU���������У���OpenGLʹ��
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		// 4. �趨��������ָ�룬�����Ͷ��㻺���еĶ������Բ���
		glEnableVertexAttribArray(0);// ����glsl��layout = 0����
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

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
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			// 5.�󶨶���������󣬲�����-����Ҫ����ɫ����Ĭ��һ����ɫ����ɫ��
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			// ��ǰ����˳����²�
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

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