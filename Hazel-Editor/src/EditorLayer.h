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

	private:
		OrthographicsCameraController m_CameraController;
		Ref<FrameBuffer> m_FrameBuffer; // Ö¡»º³å
		glm::vec2 m_ViewportSize;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Ref<SceneHierarchyPanel> m_SceneHierarchyPanel;
	};
}

