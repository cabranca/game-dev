#include <pch.h>
#include "OpenGLShader.h"

#include <glad/glad.h>

#include <Cabrankengine/Math/MatrixFactory.h>

namespace cabrankengine::platform::opengl {

	using namespace math;
	using namespace rendering;

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

		GLint location = glGetUniformLocation(m_RendererId, name.data());
		if (location == -1) {
			CE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
    	glProgramUniform1i(m_RendererId, location, value);
	}

	void OpenGLShader::setIntArray(const std::string& name, uint32_t count, int* values) {
		CE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererId, name.data());
		if (location == -1) {
			CE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glProgramUniform1iv(m_RendererId, location, count, values);
	}

	void OpenGLShader::setFloat(const std::string& name, float value) {
		CE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererId, name.data());
		if (location == -1) {
			CE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glProgramUniform1f(m_RendererId, location, value);
	}

	void OpenGLShader::setFloat2(const std::string& name, Vector2 value) {
		CE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererId, name.data());
		if (location == -1) {
			CE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glProgramUniform2f(m_RendererId, location, value.x, value.y);
	}

	void OpenGLShader::setFloat3(const std::string& name, Vector3 values) {
		CE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererId, name.data());
		if (location == -1) {
			CE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glProgramUniform3f(m_RendererId, location, values.x, values.y, values.z);
	}

	void OpenGLShader::setFloat4(const std::string& name, const Vector4& values) {
		CE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererId, name.data());
		if (location == -1) {
			CE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glProgramUniform4f(m_RendererId, location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::setMat4(const std::string& name, const Mat4& matrix) {
		CE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererId, name.data());
		if (location == -1) {
			CE_CORE_ERROR("Uniform {0} not found in shader!", name);
			return;
		}
		glProgramUniformMatrix4fv(m_RendererId, location, 1, GL_FALSE, reinterpret_cast<const float*>(matrix.elements.data()));
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
