#include "pch.h"
#include "Hazel/Core/Layer.h"
#include "Hazel/Core/Log.h"

namespace Hazel
{
	Layer::Layer(const std::string name)
		: m_DebugName(name)
	{
		HZ_CORE_INFO("Creating Layer:[{0}]", name);
	}
} // namespace Hazel