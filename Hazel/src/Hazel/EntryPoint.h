#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
	Hazel::Log::Init();
	// 原本代码输出日志
	Hazel::Log::GetCoreLogger()->warn("原始代码输出日志");
	Hazel::Log::GetClientLogger()->error("原始代码输出日志");

	// 用宏定义，输出日志
	HZ_CORE_WARN("Initialized Log!");
	int a = 5;
	HZ_INFO("Hello! Var={0}", a);

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif