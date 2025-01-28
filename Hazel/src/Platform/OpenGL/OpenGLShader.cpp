#include "pch.h"
#include "Hazel/Core/Log.h"
#include "OpenGLShader.h"
#include <glad/glad.h>
#include <filesystem>

namespace Hazel
{
	// Helper function to map string type to GLenum type
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		static const std::unordered_map<std::string, GLenum> shaderTypeMap = {
			{"vertex", GL_VERTEX_SHADER},
			{"fragment", GL_FRAGMENT_SHADER},
			{"pixel", GL_FRAGMENT_SHADER}
		};

		auto it = shaderTypeMap.find(type);
		if (it != shaderTypeMap.end()) {
			return it->second;
		}

		HZ_CORE_ASSERT(false, "Unknown shader type: {0}", type);
		return 0;  // This won't be reached if the assertion is triggered
	}

	OpenGLShader::OpenGLShader(const std::string name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		// Construct the shader sources map
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;

		// Create the shader program
		m_RendererID = CreateShader(shaderSources);

		HZ_CORE_INFO("Loaded shader: {0}", name);
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);

		// Create the shader program
		m_RendererID = CreateShader(shaderSources);

		std::filesystem::path path(filepath);
		std::string shaderName = path.stem().string();

		m_Name = shaderName;

		HZ_CORE_INFO("Loaded shader: {0}", shaderName);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetTextureSlot(std::string textureName, uint32_t slot)
	{
		UploadUniform1i(textureName, slot);
	}

	void OpenGLShader::SetInt(std::string name, uint32_t value)
	{
		UploadUniform1i(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, const int* value, const uint32_t count)
	{
		UploadUniformIntArray(name, value, count);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		UploadUniform1f(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		UploadUniform2f(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		UploadUniform3f(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		UploadUniform4f(name, value);
	}

	void OpenGLShader::SetMat3(std::string name, glm::mat3 value)
	{
		UploadUniformMat3(name, value);
	}

	void OpenGLShader::SetMat4(std::string name, glm::mat4 value)
	{
		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniform1f(const std::string& name, float value)
	{
		glUseProgram(m_RendererID); // 为了避免绑定
		GLint location = GetUniformLocation(name);
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniform1i(const std::string& name, int value)
	{
		glUseProgram(m_RendererID);
		GLint location = GetUniformLocation(name);
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, const int* value, const uint32_t count)
	{
		glUseProgram(m_RendererID);
		GLint location = GetUniformLocation(name);
		glUniform1iv(location, count, value);
	}

	void OpenGLShader::UploadUniform2f(const std::string& name, const glm::vec2& value)
	{
		glUseProgram(m_RendererID);
		GLint location = GetUniformLocation(name);
		glUniform2fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniform3f(const std::string& name, const glm::vec3& value)
	{
		glUseProgram(m_RendererID);
		GLint location = GetUniformLocation(name);
		glUniform3fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniform4f(const std::string& name, const glm::vec4& value)
	{
		glUseProgram(m_RendererID);
		GLint location = GetUniformLocation(name);
		glUniform4fv(location, 1 , glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& value)
	{
		glUseProgram(m_RendererID);
		GLint location = GetUniformLocation(name);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& value)
	{
		glUseProgram(m_RendererID);
		GLint location = GetUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	uint32_t OpenGLShader::CreateShader(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		// Create the program
		uint32_t program = glCreateProgram();

		// Compile and attach each shader
		std::vector<uint32_t> shaderIDs;
		for (const auto& [type, source] : shaderSources)
		{
			uint32_t shaderID = CompileShader(type, source);
			glAttachShader(program, shaderID);
			shaderIDs.push_back(shaderID);
		}

		// Link the program
		glLinkProgram(program);

		// Check for linking errors
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// Clean up shaders
			for (auto shaderID : shaderIDs)
			{
				glDeleteShader(shaderID);
			}

			glDeleteProgram(program);
			HZ_CORE_ERROR("Shader linking failed: {0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Program linking failed!");
			return 0;
		}


		// Clean up shaders (no longer needed after linking)
		for (auto shaderID : shaderIDs)
		{
			glDetachShader(program, shaderID);
			glDeleteShader(shaderID);
		}

		return program;
	}

	uint32_t OpenGLShader::CompileShader(GLenum type, const std::string& source)
	{
		// Create the shader object
		uint32_t shader = glCreateShader(type);

		// Set shader source code and compile
		const char* src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		// Check for compilation errors
		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			// Retrieve error log
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			// Delete shader as it's not valid
			glDeleteShader(shader);

			// Determine shader type as a string
			std::string shaderType;
			switch (type)
			{
			case GL_VERTEX_SHADER: shaderType = "Vertex"; break;
			case GL_FRAGMENT_SHADER: shaderType = "Fragment"; break;
			case GL_GEOMETRY_SHADER: shaderType = "Geometry"; break;
			case GL_COMPUTE_SHADER: shaderType = "Compute"; break;
			default: shaderType = "Unknown"; break;
			}

			// Log the error with more detailed information
			HZ_CORE_ERROR("{} Shader compilation failed:\n{}", shaderType, infoLog.data());
			HZ_CORE_ASSERT(false, "Shader compilation failure!");

			return 0;
		}

		return shader;
	}

	int OpenGLShader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
		{
			HZ_CORE_WARN("Uniform '{0}' not found or not used!", name);
			return -1; // 确保程序不会崩溃
		}
		m_UniformLocationCache[name] = location;
		return location;
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (!in) {
			HZ_CORE_ERROR("Failed to open file: {0}", filepath);
			return "";
		}

		in.seekg(0, std::ios::end);
		std::streamsize size = in.tellg();
		in.seekg(0, std::ios::beg);
		if (size <= 0) {
			HZ_CORE_ERROR("File is empty: {0}", filepath);
			return "";
		}

		std::vector<char> buffer(size);
		in.read(buffer.data(), size);

		return std::string(buffer.begin(), buffer.end());
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		std::string typeToken = "#type";
		size_t typeTokenLen = typeToken.size();
		size_t findCurPos = source.find(typeToken, 0);
		
		size_t findNextPos = findCurPos;

		while (findCurPos != std::string::npos)
		{
			// Find the end of the current line to get the shader type
			size_t curLineEndPos = source.find_first_of("\r\n", findCurPos);
			HZ_CORE_ASSERT(curLineEndPos != std::string::npos, "Shader parse error: could not find end of line");

			// Extract the shader type (vertex, fragment, etc.)
			size_t begin = findCurPos + typeTokenLen + 1;
			std::string type = source.substr(begin, curLineEndPos - begin);

			// Use the helper function for type validation
			GLenum shaderType = ShaderTypeFromString(type);

			// Find the next shader type declaration
			size_t nextLinePos = source.find_first_not_of("\r\n", curLineEndPos);
			size_t findNextPos = source.find(typeToken, nextLinePos);

			// Extract shader source code between current #type and next #type or end of file
			shaderSources[shaderType] = source.substr(nextLinePos, findNextPos - nextLinePos);

			// Move to the next shader type declaration
			findCurPos = findNextPos;
		}
		return shaderSources;
	}

} // namespace Hazel