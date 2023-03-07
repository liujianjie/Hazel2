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
		HZ_CORE_ASSERT(!s_Instance, "引用已经存在");
		s_Instance = this;

		// 1.1Application创建窗口
		m_Window = std::unique_ptr<Window>(Window::Create());
		// 1.2Application设置窗口事件的回调函数
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		// 将ImGui层放在最后
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// 使用OpenGL函数渲染一个三角形
		// 顶点数据
		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};
		unsigned int indices[3] = { 0, 1, 2 }; // 索引数据
		// 0.生成顶点数组对象VAO、顶点缓冲对象VBO、索引缓冲对象EBO
		glGenVertexArrays(1, &m_VertexArray);
		// 1. 绑定顶点数组对象
		glBindVertexArray(m_VertexArray);

		// 2.1顶点缓冲
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		// 2.2 设定顶点属性指针，来解释顶点缓冲中的顶点属性布局
		glEnableVertexAttribArray(0);// 开启glsl的layout = 0输入
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		// 3.索引缓冲
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		// 着色器代码
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";
		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";
		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
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
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			// 5.绑定着色器
			m_Shader->Bind();
			// 6.绑定顶点数组对象，并绘制
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			// 从前往后顺序更新层
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

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