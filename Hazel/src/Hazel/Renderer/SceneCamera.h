#pragma once

#include "Hazel/Renderer/Camera.h"

namespace Hazel
{
	class SceneCamera : public Camera
	{
	public:
		// 摄像机投影类型枚举
		enum class ProjectionType
		{
			Perspective = 0,
			Orthographic = 1
		};

	public:
		SceneCamera(ProjectionType type = ProjectionType::Perspective);
		virtual ~SceneCamera() = default;

		// 设置视口大小
		void SetViewportSize(uint32_t width, uint32_t height);

		// 正交投影设置
		void SetOrthographic(float size, float nearClip, float farClip);
		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
		void SetOrthographicNear(float value) { m_OrthographicNear = value; RecalculateProjection(); }
		void SetOrthographicFar(float value) { m_OrthographicFar = value; RecalculateProjection(); }
		float GetOrthographicNear() const { return m_OrthographicNear; }
		float GetOrthographicFar() const { return m_OrthographicFar; }
		
		// 透视投影设置
		void SetPerspective(float fov, float nearClip, float farClip);
		void SetPerspectiveFOV(float fov) { m_PerspectiveFOV = glm::radians(fov); RecalculateProjection(); }
		void SetPerspectiveNear(float value) { m_PerspectiveNear = value; RecalculateProjection(); }
		void SetPerspectiveFar(float value) { m_PerspectiveFar = value; RecalculateProjection(); }
		float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
		float GetPerspectiveNear() const { return m_PerspectiveNear; }
		float GetPerspectiveFar() const { return m_PerspectiveFar; }
		
		// 获取当前的投影类型
		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		// 当前投影类型
		ProjectionType m_ProjectionType = ProjectionType::Perspective;

		// 正交投影参数
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;

		// 透视投影参数
		float m_PerspectiveFOV = glm::radians(45.0f);  // 45度的FOV
		float m_PerspectiveNear = 0.0f;
		float m_PerspectiveFar = 1000.0f;

		// 视口宽高比
		float m_AspectRatio = 0.0f;
	};
}