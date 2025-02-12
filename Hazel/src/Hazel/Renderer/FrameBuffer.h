#pragma once

#include "Hazel/Core/Core.h"

namespace Hazel
{
	// 帧缓冲的规格说明
	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1; // 默认 1x 采样

		bool SwapChainTarget = false; // 是否为默认帧缓冲（屏幕）
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