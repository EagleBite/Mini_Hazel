#pragma once

#include "Hazel.h"

namespace Hazel
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;

		bool IsViewportFocused() const { return m_ViewportFocused; }
		bool IsViewportHovered() const { return m_ViewportHovered; }
	private:
		OrthographicsCameraController m_CameraController; // Õý½»ÉãÏñ»ú¿ØÖÆÆ÷
		Ref<FrameBuffer> m_FrameBuffer; // Ö¡»º³å
		glm::vec2 m_ViewportSize = glm::vec2(0.0f);

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Ref<SceneHierarchyPanel> m_SceneHierarchyPanel;
	};
}

