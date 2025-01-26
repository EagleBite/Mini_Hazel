#pragma once

#include <glm/glm.hpp>

namespace Hazel
{
	// 透视投影摄像机
	class PerspectiveGraphicsCamera
	{
	public:
		PerspectiveGraphicsCamera() {}
		PerspectiveGraphicsCamera(float fov, float aspectRatio, float nearClip, float farClip);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		float GetFOV() const { return m_FOV; }
		void SetFOV(float fov) { m_FOV = fov; RecalculateProjectionMatrix(); }

		float GetAspectRatio() const { return m_AspectRatio; }
		void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; RecalculateProjectionMatrix(); }

		float GetNearClip() const { return m_NearClip; }
		float GetFarClip() const { return m_FarClip; }
		void SetClippingPlanes(float nearClip, float farClip)
		{
			m_NearClip = nearClip;
			m_FarClip = farClip;
			RecalculateProjectionMatrix();
		}

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateProjectionMatrix();
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f; // 角度制

		float m_FOV = 45.0f; // 视场角（角度制）
		float m_AspectRatio = 1.7778f; // 默认宽高比（16:9）
		float m_NearClip = 0.1f;
		float m_FarClip = 100.0f;
	};


} // namespace Hazel