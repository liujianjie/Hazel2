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
		void OnEvent(Event& e);			// 回调窗口事件函数
		void PushLayer(Layer* layer);	// 添加层
		void PushOverlay(Layer* layer); // 添加覆盖层
		inline Window& GetWindow() { return *m_Window; }// 获得Window
		inline static Application& Get() { return *s_Instance; }// 获取单例
	private:
		bool OnWindowClose(WindowCloseEvent& e); // 处理窗口关闭事件的函数
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