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
		 * �Ӿ�����(Sprite Sheet)����������
		 * @param texture  ԭʼ����
		 * @param coords   �����ڱ���е����꣨�Ը���Ϊ��λ��
		 * @param cellSize ÿ����Ԫ��Ĵ�С�����ص�λ��
		 * @param spriteSize  ����ĳߴ磨�Ը���Ϊ��λ��Ĭ��Ϊ 1x1��
		 */
		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1, 1 });
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TextureCoords[4];
	};
}