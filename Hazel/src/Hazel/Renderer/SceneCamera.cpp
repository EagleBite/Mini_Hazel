#include "pch.h"
#include "Hazel/Core/Log.h"
#include "SceneCamera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel
{
	SceneCamera::SceneCamera(ProjectionType type /*= ProjectionType::Perspective*/)
		: m_ProjectionType(type)
	{
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		RecalculateProjection();
	}

	/*
	* 备注: 传递的fov参数是角度制 计算使用的弧度制
	*/
	void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
	{
		m_PerspectiveFOV = glm::radians(fov);  // 将角度转换为弧度
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Orthographic)
		{
			// 正交投影矩阵
			m_ProjectionMatrix = glm::ortho(
				-m_OrthographicSize * m_AspectRatio, m_OrthographicSize * m_AspectRatio,
				-m_OrthographicSize, m_OrthographicSize,
				m_OrthographicNear, m_OrthographicFar
			);
		}
		else
		{
			// 透视投影矩阵
			m_ProjectionMatrix = glm::perspective(
				m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar
			);
		}
	}

}