#include <Hazel.h>
#include "imgui/imgui.h"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}
	void OnUpdate() override
	{
		//HZ_INFO("ExampleLayer::Update");
		if (Hazel::Input::IsKeyPressed(HZ_KEY_A)) {
			HZ_TRACE("A键按下(poll)");
		}
	}
	void OnEvent(Hazel::Event& event) override
	{
		//HZ_TRACE("examplayer:{0}", event);
		if (event.GetEventType() == Hazel::EventType::KeyPressed) {
			Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
			if (e.GetKeyCode() == HZ_KEY_A) {
				HZ_TRACE("A键按下(event)");
			}
			HZ_TRACE("{0}",(char)e.GetKeyCode());
		}
	}
	// 每个层都可以有自己的UI窗口
	virtual void OnImGuiRender() override{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}
};
class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		//PushOverlay(new Hazel::ImGuiLayer());// UI层放到最后面显示在屏幕的上方
	}
	~Sandbox()
	{
	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}