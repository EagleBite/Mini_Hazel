#include "pch.h"
#include "OrthographicsCameraController.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"

namespace Hazel
{

	OrthographicsCameraController::OrthographicsCameraController(float aspectRatio) : 
		m_AspectRatio(aspectRatio), 
		m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), 
		m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top)
	{

	}

	void OrthographicsCameraController::OnUpdate(Timestep ts)
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_W)) {
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_S)) {
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		}
		if (Hazel::Input::IsKeyPressed(HZ_KEY_A)) {
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_D)) {
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		}
		if (Hazel::Input::IsKeyPressed(HZ_KEY_E)) {
			m_CameraRotation += m_CameraRotationSpeed * ts;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_Q)) {
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		}

		m_Camera.SetRotation(m_CameraRotation);
		m_Camera.SetPosition(m_CameraPosition);
	}

	void OrthographicsCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicsCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicsCameraController::OnWindowResized));
	}

	void OrthographicsCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height; // ����������
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	bool OrthographicsCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
		return false;
	}

	bool OrthographicsCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

}