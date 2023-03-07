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

		// 渲染一个三角形所做的准备
		// 0.顶点数据
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		unsigned int indices[3] = { 0, 1, 2 }; // 索引数据
		// 1.生成顶点数组对象VAO
		m_VertexArray.reset(VertexArray::Create());
		// 2.顶点缓冲
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		// 设定顶点属性指针，来解释顶点缓冲中的顶点属性布局
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		// 3.先设置顶点缓冲布局-计算好各个属性的所需的值
		vertexBuffer->SetLayout(layout);
		// 4.再给顶点数组添加顶点缓冲-设置各个属性的顶点属性指针
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		// 5.索引缓冲
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		// 6.给顶点数组设置索引缓冲
		m_VertexArray->SetIndexBuffer(indexBuffer);
		// 着色器代码
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";
		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";
		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		// 渲染一个quad所做的准备
		// 0.顶点数据
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 }; // 索引数据
		// 1.生成顶点数组对象VAO
		m_SquareVA.reset(VertexArray::Create());
		// 2.顶点缓冲
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		// 设定顶点属性指针，来解释顶点缓冲中的顶点属性布局
		BufferLayout layout2 = {
			{ ShaderDataType::Float3, "a_Position" }
		};
		// 3.先设置顶点缓冲布局-计算好各个属性的所需的值
		squareVB->SetLayout(layout2);
		// 4.再给顶点数组添加顶点缓冲-设置各个属性的顶点属性指针
		m_SquareVA->AddVertexBuffer(squareVB);
		// 5.索引缓冲
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		// 6.给顶点数组设置索引缓冲
		m_SquareVA->SetIndexBuffer(squareIB);
		// 着色器代码
		std::string blueShaderVertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";
		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";
		m_BlueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
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
			// 绘制四边形
			m_BlueShader->Bind();// 绑定着色器
			m_SquareVA->Bind();// 绑定顶点数组对象，并绘制
			glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			// 绘制三角形
			m_Shader->Bind();// 绑定着色器
			m_VertexArray->Bind();// 绑定顶点数组对象，并绘制
			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

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