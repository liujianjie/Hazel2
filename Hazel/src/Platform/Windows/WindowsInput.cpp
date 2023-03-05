#include "hzpch.h"
#include "WindowsInput.h"

#include "Hazel/Application.h"
#include <GLFW/glfw3.h>

namespace Hazel {
	// ����ָ��ָ���������
	Input* Input::s_Instance = new WindowsInput();// ���徲̬����ȫ�ֶ���

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		// ��ȡGLFWԭ������void*��תΪGLFWwindow*
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		// �����е�GLFW��������ȡ����״̬
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		// C++17д��
		auto [x, y] = GetMousePositionImpl();
		return x;
		// C++14����
		//auto x = GetMousePositionImpl();
		//return std::get<0>(x);
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}

}