#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/TimeStep.h"
#include "Hazel/Renderer/OrthoGraphicsCamera.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel
{
	struct OrthographicsCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	// ���������������
	class OrthographicsCameraController
	{
	public:
		OrthographicsCameraController(float aspectRatio);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthoGraphicsCamera& GetCamera() { return m_Camera; }
		const OrthoGraphicsCamera& GetCamera() const { return m_Camera; }

		const OrthographicsCameraBounds GetBounds() { return m_Bounds; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio;       // �����
		float m_ZoomLevel = 1.0f;  // ����
		OrthoGraphicsCamera m_Camera;
		OrthographicsCameraBounds m_Bounds;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraMoveSpeed = 5.0f;
		float m_CameraRotationSpeed = 30.0f;
	};

} // namespace Hazel