#pragma once

#include "Texture.h"
#include <glm/glm.hpp>

namespace Hazel
{
	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const Ref<Texture2D> GetTexture()const { return m_Texture; }
		const glm::vec2* GetTexCoords() const { return m_TextureCoords; }

		/**
		 * 从精灵表格(Sprite Sheet)创建子纹理。
		 * @param texture  原始纹理
		 * @param coords   精灵在表格中的坐标（以格子为单位）
		 * @param cellSize 每个单元格的大小（像素单位）
		 * @param spriteSize  精灵的尺寸（以格子为单位，默认为 1x1）
		 */
		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1, 1 });
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TextureCoords[4];
	};
}