#pragma once

#include "Hazel/Core.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/Event.h"

namespace Hazel {

	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {} // Ӧ����Ӵ˲�ִ��
		virtual void OnDetach() {} // Ӧ�÷���˲�ִ��
		virtual void OnUpdate(Timestep ts) {} // ÿ�����
		virtual void OnImGuiRender() {}// ÿ�㶼����ӵ���Լ���UI
		virtual void OnEvent(Event& event) {}// ÿ�㴦���¼�

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}