#pragma once

#include <cstdint>
#include <unordered_map>
#include <glm/glm.hpp>
#include <Cabrankengine/Renderer/Shader.h>

// TODO: Remove!!
typedef unsigned int GLenum;

namespace cabrankengine {

	class OpenGLShader : public Shader {
		public:
			OpenGLShader(const std::string& filepath);
			OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
			virtual ~OpenGLShader();

			// Binds the shader program for use in rendering.
			virtual void bind() const override;

			// Unbinds the shader program, stopping its use in rendering.
			virtual void unbind() const override;

			// Returns the name of the shader program given by the user.
			const std::string& getName() const override { return m_Name; }

			// Uploads a uniform value to the shader program.
			void uploadUniformInt(const std::string& name, int value);
			void uploadUniformFloat1(const std::string& name, float value);
			void uploadUniformFloat2(const std::string& name, const glm::vec2& values);
			void uploadUniformFloat3(const std::string& name, const glm::vec3& values);
			void uploadUniformFloat4(const std::string& name, const glm::vec4& values);
			void uploadUniformMat3(const std::string& name, const glm::mat3& matrix);
			void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		private:
			// Reads the shader source code from a file.
			std::string readFile(const std::string& filepath);
			
			// Pre-processes the shader source code to retrieve the individual shader programs (vertex, fragment, etc.).
			std::unordered_map<GLenum, std::string> preProcess(const std::string& source);
			
			// Compiles the shader source code into a shader program.
			void compile(const std::unordered_map<GLenum, std::string>& shaderSources);

			uint32_t m_RendererId; // OpenGL renderer ID for the shader program
			std::string m_Name; // Name of the shader
	};
}