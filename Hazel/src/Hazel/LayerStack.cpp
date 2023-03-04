#include "hzpch.h"
#include "LayerStack.h"
namespace Hazel {
	LayerStack::LayerStack()
	{
		m_LayerInsert = m_Layers.begin();
	}
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
			delete layer;
	}
	
	void LayerStack::PushLayer(Layer* layer)
	{
		// emplace��vector����ָ��λ��֮ǰ����һ���µ�Ԫ�ء����ز���Ԫ�ص�λ��
		// ���� 1 2 3��vector�� 3 2 1
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
	}
	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}
	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsert--;	// ָ��Begin
		}
	}
	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
			m_Layers.erase(it);
	}
}