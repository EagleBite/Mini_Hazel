#include "pch.h"
#include "SubTexture2D.h"

namespace Hazel
{
	/**
	 * @brief ����������ָ�� UV ��Χ
	 * @param texture  Ŀ�������
	 * @param min      �������½� UV ����
	 * @param max      �������Ͻ� UV ����
	 */
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		// ������������ĸ� UV �����꣨���¡����¡����ϡ����ϣ�
		m_TextureCoords[0] = { min.x, min.y }; // ����
		m_TextureCoords[1] = { max.x, min.y }; // ����
		m_TextureCoords[2] = { max.x, max.y }; // ����
		m_TextureCoords[3] = { min.x, max.y }; // ����
	}

	/**
	 * @brief ������������� UV ���꣬����������
	 * @param texture   Ŀ�������
	 * @param coords    �����������е����� (��, ��)
	 * @param cellSize  ��Ԫ���С�����أ�
	 * @param spriteSize ����ռ�ݵ�����Ԫ����Ĭ�� 1x1��
	 * @return ����ָ�룬ָ���´����� `SubTexture2D` ����
	 */
	Hazel::Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize /*= { 1, 1 }*/)
	{
		// ���� UV ���꣨���½ǣ�
		glm::vec2 min = {
			(coords.x * cellSize.x) / texture->GetWidth(),
			(coords.y * cellSize.y) / texture->GetHeight()
		};

		// ���� UV ���꣨���Ͻǣ�
		glm::vec2 max = {
			((coords.x + spriteSize.x) * cellSize.x) / texture->GetWidth(),
			((coords.y + spriteSize.y) * cellSize.y) / texture->GetHeight()
		};
		return CreateRef<SubTexture2D>(texture, min, max);
	}

}