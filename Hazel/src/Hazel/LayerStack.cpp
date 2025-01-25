#include "pch.h"
#include "Hazel/LayerStack.h"
#include "Hazel/Log.h"

namespace Hazel
{
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		// 添加到尾部
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++; // 调整插入位置
	}

	void LayerStack::PushOverlay(Layer* layer)
	{
		// 添加一个覆盖层
		m_Layers.emplace_back(layer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--; // 调整插入位置
		}
	}

	void LayerStack::PopOverlay(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
		}
	}

}