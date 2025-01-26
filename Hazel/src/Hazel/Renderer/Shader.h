#pragma once

#include "Hazel/Log.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel
{

	class Shader
	{
	public:
		Shader() : m_RendererID(0) {}
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void UnBind() const;

		// Uniform �ϴ�����
		void UploadUniform1f(const std::string& name, float value);              // �ϴ�����float
		void UploadUniform1i(const std::string& name, int value);                // �ϴ�����int
		void UploadUniform2f(const std::string& name, const glm::vec2& value);   // �ϴ�vec2
		void UploadUniform3f(const std::string& name, const glm::vec3& value);   // �ϴ�vec3
		void UploadUniform4f(const std::string& name, const glm::vec4& value);   // �ϴ�vec4
		void UPloadUniformMat3(const std::string& name, const glm::mat3& value); // �ϴ�mat3
		void UploadUniformMat4(const std::string& name, const glm::mat4& value); // �ϴ�mat4


	private:
		uint32_t CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		uint32_t CompileShader(unsigned int type, const std::string& source);
		int GetUniformLocation(const std::string& name) const;

	private:
		uint32_t  m_RendererID;
	};

} // namespace Hazel