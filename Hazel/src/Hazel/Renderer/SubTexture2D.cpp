#include "pch.h"
#include "SubTexture2D.h"

namespace Hazel
{
	/**
	 * @brief 构造子纹理，指定 UV 范围
	 * @param texture  目标大纹理
	 * @param min      纹理左下角 UV 坐标
	 * @param max      纹理右上角 UV 坐标
	 */
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		// 计算子纹理的四个 UV 角坐标（左下、右下、右上、左上）
		m_TextureCoords[0] = { min.x, min.y }; // 左下
		m_TextureCoords[1] = { max.x, min.y }; // 右下
		m_TextureCoords[2] = { max.x, max.y }; // 右上
		m_TextureCoords[3] = { min.x, max.y }; // 左上
	}

	/**
	 * @brief 从网格坐标计算 UV 坐标，创建子纹理
	 * @param texture   目标大纹理
	 * @param coords    精灵在网格中的坐标 (列, 行)
	 * @param cellSize  单元格大小（像素）
	 * @param spriteSize 精灵占据的网格单元数（默认 1x1）
	 * @return 共享指针，指向新创建的 `SubTexture2D` 对象
	 */
	Hazel::Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize /*= { 1, 1 }*/)
	{
		// 计算 UV 坐标（左下角）
		glm::vec2 min = {
			(coords.x * cellSize.x) / texture->GetWidth(),
			(coords.y * cellSize.y) / texture->GetHeight()
		};

		// 计算 UV 坐标（右上角）
		glm::vec2 max = {
			((coords.x + spriteSize.x) * cellSize.x) / texture->GetWidth(),
			((coords.y + spriteSize.y) * cellSize.y) / texture->GetHeight()
		};
		return CreateRef<SubTexture2D>(texture, min, max);
	}

}