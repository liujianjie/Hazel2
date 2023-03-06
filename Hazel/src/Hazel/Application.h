#pragma once

#include "Core.h"
#include "Window.h"
#include "Hazel/LayerStack.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "Hazel/Renderer/Shader.h"

namespace Hazel {
	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);			// �ص������¼�����
		void PushLayer(Layer* layer);	// ��Ӳ�
		void PushOverlay(Layer* layer); // ��Ӹ��ǲ�
		inline Window& GetWindow() { return *m_Window; }// ���Window
		inline static Application& Get() { return *s_Instance; }// ��ȡ����
	private:
		bool OnWindowClose(WindowCloseEvent& e); // �����ڹر��¼��ĺ���
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
	private:
		static Application* s_Instance;
	};
	// To be defined in CLIENT
	Application* CreateApplication();
}