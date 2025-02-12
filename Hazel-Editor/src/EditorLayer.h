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
		Ref<FrameBuffer> m_FrameBuffer; // ֡����
		glm::vec2 m_ViewportSize;

		bool m_ViewportFocused;
		bool m_ViewportHovered;

		// ��ʱ����������
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_SpriteSheet; // ����ͼ
		Ref<SubTexture2D> m_TextureStair;
		Ref<SubTexture2D> m_TextureBush;
		Ref<SubTexture2D> m_TextureTree;

		
	};
}

