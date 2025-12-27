#include <pch.h>
#include "MetalShader.h"

#include <Cabrankengine/Math/MatrixFactory.h>

namespace cabrankengine::platform::metal {

	using namespace math;
	using namespace rendering;

	MetalShader::MetalShader(const std::string& filepath) : m_RendererId(0) {
		CE_PROFILE_FUNCTION();

		std::filesystem::path path(filepath);
		m_Name = path.stem().string();

		std::string shaderSource = readFile(filepath);
		auto shaderSources = preProcess(shaderSource);
		compile(shaderSources);
	}

	MetalShader::MetalShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) 
		: m_RendererId(0), m_Name(name) {
		CE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		compile(sources);
	}

	MetalShader::~MetalShader() {
		CE_PROFILE_FUNCTION();

	}

	void MetalShader::bind() const {
		CE_PROFILE_FUNCTION();

	}

	void MetalShader::unbind() const {
		CE_PROFILE_FUNCTION();

	}

	void MetalShader::setInt(const std::string& name, int value) {
		CE_PROFILE_FUNCTION();

	}

	void MetalShader::setIntArray(const std::string& name, uint32_t count, int* values) {
		CE_PROFILE_FUNCTION();

	}

	void MetalShader::setFloat(const std::string& name, float value) {
		CE_PROFILE_FUNCTION();

	}

	void MetalShader::setFloat3(const std::string& name, const Vector3& vector) {
		CE_PROFILE_FUNCTION();

	}

	void MetalShader::setFloat4(const std::string& name, const Vector4& vector) {
		CE_PROFILE_FUNCTION();

	}

	void MetalShader::setMat4(const std::string& name, const Mat4& value) {
		CE_PROFILE_FUNCTION();

	}

	std::string MetalShader::readFile(const std::string& filepath) {
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

	std::unordered_map<GLenum, std::string> MetalShader::preProcess(const std::string& source) {
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
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
		}
		return shaderSources;
	}

	void MetalShader::compile(const std::unordered_map<GLenum, std::string>& shaderSources) {
		CE_PROFILE_FUNCTION();

	}
}
