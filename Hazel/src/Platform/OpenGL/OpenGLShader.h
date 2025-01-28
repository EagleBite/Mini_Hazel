#pragma once

#include "Hazel/Renderer/Shader.h"
#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace Hazel
{

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& filepath);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual std::string GetName() const override { return m_Name; }

		virtual void SetTextureSlot(std::string textureName, uint32_t slot) override;
		virtual void SetInt(std::string name, uint32_t value) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat3(std::string name, glm::mat3 value) override;
		virtual void SetMat4(std::string name, glm::mat4 value) override;

	private:
		// Uniform �ϴ�����
		void UploadUniform1f(const std::string& name, float value);              // �ϴ�����float
		void UploadUniform1i(const std::string& name, int value);                // �ϴ�����int
		void UploadUniform2f(const std::string& name, const glm::vec2& value);   // �ϴ�vec2
		void UploadUniform3f(const std::string& name, const glm::vec3& value);   // �ϴ�vec3
		void UploadUniform4f(const std::string& name, const glm::vec4& value);   // �ϴ�vec4
		void UploadUniformMat3(const std::string& name, const glm::mat3& value); // �ϴ�mat3
		void UploadUniformMat4(const std::string& name, const glm::mat4& value); // �ϴ�mat4

	private:
		uint32_t CreateShader(const std::unordered_map<GLenum, std::string>& shaderSources);
		//uint32_t CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		uint32_t CompileShader(GLenum type, const std::string& source);
		int GetUniformLocation(const std::string& name);
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);

	private:
		uint32_t  m_RendererID;
		std::string m_Name;
		std::unordered_map<std::string, int> m_UniformLocationCache; // ����
	};

} // namespace Hazel