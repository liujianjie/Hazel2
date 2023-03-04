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
		// emplace在vector容器指定位置之前插入一个新的元素。返回插入元素的位置
		// 插入 1 2 3，vector是 3 2 1
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
			m_LayerInsert--;	// 指向Begin
		}
	}
	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
			m_Layers.erase(it);
	}
}