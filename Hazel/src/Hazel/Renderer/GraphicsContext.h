#pragma once

namespace Hazel
{
	// ͼ��������Context
	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

} // namespace Hazel
