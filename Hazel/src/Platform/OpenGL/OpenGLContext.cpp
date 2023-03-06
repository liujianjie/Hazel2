#include "hzpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>
namespace Hazel {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		// �����Ǵ��ڵ�����������Ϊ��ǰ�̵߳���������
		glfwMakeContextCurrent(m_WindowHandle);
		// ��ȡ�Կ�OpenGL��������ĵ�ַ
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

		HZ_CORE_INFO("OpenGL ��Ϣ:");
		HZ_CORE_INFO("	Vendor��{0}", (const char*)glGetString(GL_VENDOR));
		HZ_CORE_INFO("	�Կ�����{0}", (const char*)glGetString(GL_RENDERER));
		HZ_CORE_INFO("	�汾��{0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);// ��������
	}
}