#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
	Hazel::Log::Init();
	// ԭ�����������־
	Hazel::Log::GetCoreLogger()->warn("ԭʼ���������־");
	Hazel::Log::GetClientLogger()->error("ԭʼ���������־");

	// �ú궨�壬�����־
	HZ_CORE_WARN("Initialized Log!");
	int a = 5;
	HZ_INFO("Hello! Var={0}", a);

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif