#pragma once

#include "Hazel.h"

class SandBox2D : public Hazel::Layer
{
public:
	SandBox2D();
	virtual ~SandBox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnEvent(Hazel::Event& event) override;
	virtual void OnImGuiRender() override;

private:
	glm::vec4 m_Color = glm::vec4(1, 0, 0, 1);

	Hazel::OrthographicsCameraController m_CameraController;
	Hazel::Ref<Hazel::Texture2D> m_Texture;
};