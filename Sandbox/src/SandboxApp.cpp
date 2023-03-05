#include <Hazel.h>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		HZ_INFO("ExampleLayer::Update");
	}

	void OnEvent(Hazel::Event& event) override
	{
		HZ_TRACE("examplayer:{0}", event);
	}
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Hazel::ImGuiLayer());// UI层放到最后面显示在屏幕的上方
	}
	~Sandbox()
	{
	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}