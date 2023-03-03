#include "Application.h"
#include "Hazel/Events/ApplicationEvent.h" // 包含事件
#include "Hazel/Log.h"

namespace Hazel {
	Application::Application() {}
	Application::~Application() {}
	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);	// 使用自定义事件
		if (e.IsInCategory(EventCategoryApplication))	// 判断是否对应的分类
		{
			HZ_TRACE(e);	// 输出事件
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			HZ_TRACE(e);
		}
		while (true);
	}
}