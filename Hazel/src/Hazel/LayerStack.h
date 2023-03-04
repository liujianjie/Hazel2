#pragma once

#include "Hazel/Core.h"
#include "Layer.h"
#include <vector>
namespace Hazel {
	class HAZEL_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);	// vector��ͷ�����һ����
		void PushOverlay(Layer* overlay);// ��vectorĩβ���һ�����ǲ㣬����Ļ�����Ϸ��Ĳ�
		void PopLayer(Layer* layer);	// vector����ָ����
		void PopOverlay(Layer* overlay);// vector�������ǲ�

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsert;
	};
}