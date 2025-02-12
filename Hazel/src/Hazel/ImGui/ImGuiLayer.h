#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Layer.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/ApplicationEvent.h"

namespace Hazel
{

	class HAZEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		void BlockEvents(bool block) { m_BlockEvents = block; }

		void Begin();
		void End();
		
	private:
		float m_Time = 0;
		bool m_BlockEvents = false;
	};

} // namespace Hazel