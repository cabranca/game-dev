#include <Platform/OpenGL/OpenGLShader.h>

#include <glad/glad.h>
#include <array>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <filesystem>
#include <Cabrankengine/Core/Logger.h>
#include <Cabrankengine/Debug/Instrumentator.h>

namespace cabrankengine {

	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		else if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		CE_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath) : m_RendererId(0) {
		CE_PROFILE_FUNCTION();

		std::filesystem::path path(filepath);
		m_Name = path.stem().string();

		std::string shaderSource = readFile(filepath);
		auto shaderSources = preProcess(shaderSource);
		compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) 
		: m_RendererId(0), m_Name(name) {
		CE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		compile(sources);
	}

	OpenGLShader::~OpenGLShader() {
		CE_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererId);
	}

	void OpenGLShader::bind() const {
		CE_PROFILE_FUNCTION();

		glUseProgram(m_RendererId);
	}

	void OpenGLShader::unbind() const {
		CE_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::setInt(const std::string& name, int value) {
		CE_PROFILE_FUNCTION();

		uploadUniformInt(name, value);
	}

	void OpenGLShader::setFloat3(const std::string& name, const glm::vec3& vector) {
		CE_PROFILE_FUNCTION();

		uploadUniformFloat3(name, vector);
	}

	void OpenGLShader::setFloat4(const std::string& name, const glm::vec4& vector) {
		CE_PROFILE_FUNCTION();

		uploadUniformFloat4(name, vector);
	}

	void OpenGLShader::setMat4(const std::string& name, const glm::mat4& value) {
		CE_PROFILE_FUNCTION();

		uploadUniformMat4(name, value);
	}

	void OpenGLShader::uploadUniformInt(const std::string& name, int value) {
		CE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		if (location == -1) {
			CE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glUniform1i(location, value);
	}

	void OpenGLShader::uploadUniformFloat1(const std::string& name, float value) {
		CE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		if (location == -1) {
			CE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glUniform1f(location, value);
	}

	void OpenGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2& values) {
		CE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		if (location == -1) {
			CE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& values) {
		CE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		if (location == -1) {
			CE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& values) {
		CE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		if (location == -1) {
			CE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix) {
		CE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		if (location == -1) {
			CE_CORE_ERROR("Uniform {0} not found in shader!", name);
			return;
		}
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
		CE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		if (location == -1) {
			CE_CORE_ERROR("Uniform {0} not found in shader!", name);
			return;
		}
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::readFile(const std::string& filepath) {
		CE_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else {
			CE_CORE_ERROR("Could not open shader file: {0}", filepath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string& source) {
		CE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			CE_CORE_ASSERT(eol != std::string::npos, "Syntax error in shader file!");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			CE_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified!");
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - nextLinePos);
		}
		return shaderSources;
	}

	void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& shaderSources) {
		CE_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
		CE_CORE_ASSERT(shaderSources.size() <= 2, "OpenGL only supports 2 shader types (vertex and fragment) at the moment!");
		std::array<GLenum, 2> shaderIds;
		int shaderIndex = 0;
		for (const auto& [type, source] : shaderSources) {
			GLenum shaderId = glCreateShader(type);
			const char* src = source.c_str();
			glShaderSource(shaderId, 1, &src, nullptr);
			glCompileShader(shaderId);
			int isCompiled = 0;
			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE) {
				int maxLength = 0;
				glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<char> infoLog(maxLength);
				glGetShaderInfoLog(shaderId, maxLength, &maxLength, infoLog.data());
				glDeleteShader(shaderId);
				CE_CORE_ERROR("Shader compilation error: {0}", infoLog.data());
				return;
			}
			shaderIds[shaderIndex++] = shaderId;
		}

		for (uint32_t shaderId : shaderIds) {
			glAttachShader(program, shaderId);
		}
		glLinkProgram(program);
		for (uint32_t shaderId : shaderIds) {
			glDetachShader(program, shaderId);
			glDeleteShader(shaderId);
		}

		m_RendererId = program;
	}
}