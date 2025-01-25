#pragma once
#include "pch.h"
#include "Hazel/Core.h"
#include "Hazel/Layer.h"

namespace Hazel
{

	class HAZEL_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);   // 在vector尾部插入Layer
		void PushOverlay(Layer* layer); // 在最上方插入Layer--覆盖层
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};

} // namespace Hazel