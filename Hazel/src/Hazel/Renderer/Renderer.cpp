#include "hzpch.h"
#include "Renderer.h"

namespace Hazel {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix(); // 保存计算的Projection * view矩阵
	}
	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Bind();			// 着色器绑定
		shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);// 上传投影观察矩阵

		vertexArray->Bind();// 顶点数组绑定
		RenderCommand::DrawIndexed(vertexArray);// drawcall
	}
}