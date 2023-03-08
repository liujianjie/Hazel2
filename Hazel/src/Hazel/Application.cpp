#include "hzpch.h"
#include "Application.h"
#include "Hazel/Log.h"
#include "Hazel/Input.h"
#include "Hazel/Renderer/Renderer.h"
namespace Hazel {
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() : m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
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

		// ��Ⱦһ��������������׼��
		// 0.��������
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		unsigned int indices[3] = { 0, 1, 2 }; // ��������
		// 1.���ɶ����������VAO
		m_VertexArray.reset(VertexArray::Create());
		// 2.���㻺��
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		// �趨��������ָ�룬�����Ͷ��㻺���еĶ������Բ���
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		// 3.�����ö��㻺�岼��-����ø������Ե������ֵ
		vertexBuffer->SetLayout(layout);
		// 4.�ٸ�����������Ӷ��㻺��-���ø������ԵĶ�������ָ��
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		// 5.��������
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		// 6.����������������������
		m_VertexArray->SetIndexBuffer(indexBuffer);
		// ��ɫ������
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			uniform mat4 u_ViewProjection;
			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
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

		// ��Ⱦһ��quad������׼��
		// 0.��������
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 }; // ��������
		// 1.���ɶ����������VAO
		m_SquareVA.reset(VertexArray::Create());
		// 2.���㻺��
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		// �趨��������ָ�룬�����Ͷ��㻺���еĶ������Բ���
		BufferLayout layout2 = {
			{ ShaderDataType::Float3, "a_Position" }
		};
		// 3.�����ö��㻺�岼��-����ø������Ե������ֵ
		squareVB->SetLayout(layout2);
		// 4.�ٸ�����������Ӷ��㻺��-���ø������ԵĶ�������ָ��
		m_SquareVA->AddVertexBuffer(squareVB);
		// 5.��������
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		// 6.����������������������
		m_SquareVA->SetIndexBuffer(squareIB);
		// ��ɫ������
		std::string blueShaderVertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			uniform mat4 u_ViewProjection;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
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
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();
			/*
				5��6ָ������Զƽ�淶Χ
				glm::ortho(left, right, bottom, top, -1.0f, 1.0f)
				�����λ�õ�z��λ��ֻҪ-1~1֮�����
			*/
			m_Camera.SetPosition({ 0.5f, 0.5f, -0.9f });
			m_Camera.SetRotation(45.0f);

			Renderer::BeginScene(m_Camera);
			// �����ı���
			Renderer::Submit(m_BlueShader, m_SquareVA);

			// ����������
			Renderer::Submit(m_Shader, m_VertexArray);

			Renderer::EndScene();

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