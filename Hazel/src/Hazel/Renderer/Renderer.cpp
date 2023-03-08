#include "hzpch.h"
#include "Renderer.h"

namespace Hazel {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix(); // ��������Projection * view����
	}
	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Bind();			// ��ɫ����
		shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);// �ϴ�ͶӰ�۲����

		vertexArray->Bind();// ���������
		RenderCommand::DrawIndexed(vertexArray);// drawcall
	}
}