#pragma once
#include "Hazel/Core/Core.h"
#include "Hazel/Core/Log.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel
{

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual std::string GetName() const = 0;

		static Ref<Shader> Create(const std::string filePath);
		static Ref<Shader> Create(const std::string name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	// ×ÅÉ«Æ÷¿â
	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};

} // namespace Hazel