#pragma once

namespace Hazel
{
	// 图形上下文Context
	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

} // namespace Hazel
