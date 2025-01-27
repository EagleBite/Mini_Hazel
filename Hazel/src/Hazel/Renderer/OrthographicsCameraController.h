#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/TimeStep.h"
#include "Hazel/Renderer/OrthoGraphicsCamera.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel
{
	// ���������������
	class OrthographicsCameraController
	{
	public:
		OrthographicsCameraController(float aspectRatio);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthoGraphicsCamera& GetCamera() { return m_Camera; }
		const OrthoGraphicsCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio;       // �����
		float m_ZoomLevel = 1.0f;  // ����
		OrthoGraphicsCamera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraMoveSpeed = 0.5f;
		float m_CameraRotationSpeed = 30.0f;
	};

} // namespace Hazel