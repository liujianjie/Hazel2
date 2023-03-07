#pragma once
#include "RenderCommand.h"
namespace Hazel {

	class Renderer
	{
	public:
		static void BeginScene();	// ��ʼ����
		static void EndScene();		// ��������
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);// �ύ����Ķ�������
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}