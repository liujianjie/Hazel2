#pragma once
#include "RenderCommand.h"
namespace Hazel {

	class Renderer
	{
	public:
		static void BeginScene();	// 开始场景
		static void EndScene();		// 结束场景
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);// 提交物体的顶点数组
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}