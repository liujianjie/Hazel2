#include "hzpch.h"
#include "Renderer.h"

namespace Hazel {

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();					// 顶点数组绑定
		RenderCommand::DrawIndexed(vertexArray);// 调用drawcall
	}
}