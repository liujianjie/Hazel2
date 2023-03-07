#pragma once

#include "Hazel/Renderer/RendererAPI.h"

namespace Hazel {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;// ������������ɫ	
		virtual void Clear() override;								// �����Щ����
		// ͨ�������������
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};


}