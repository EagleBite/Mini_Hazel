#pragma once

#include "Hazel/Core/Core.h"

namespace Hazel
{
	// ֡����Ĺ��˵��
	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1; // Ĭ�� 1x ����

		bool SwapChainTarget = false; // �Ƿ�ΪĬ��֡���壨��Ļ��
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		const FrameBufferSpecification& GetSpecification() const { return m_Specification; }

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
	protected:
		FrameBufferSpecification m_Specification;
	};
}