#pragma once
#include "pch.h"
#include "Hazel/Core/Core.h"
#include "Hazel/Core/TimeStep.h"
#include "Hazel/Events/Event.h"

namespace Hazel
{

	class HAZEL_API Layer
	{
	public:
		Layer(const std::string name = "Layer");
		virtual ~Layer() {};

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate(Timestep ts) {};
		virtual void OnEvent(Event& event) {};
		virtual void OnImGuiRender() {};

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

} // namespace Hazle