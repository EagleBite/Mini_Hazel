#pragma once

#include "pch.h"
#include "Hazel/Log.h"

namespace Hazel
{

	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void UnBind() const;
	private:
		uint32_t  m_RendererID;
	};

} // namespace Hazel