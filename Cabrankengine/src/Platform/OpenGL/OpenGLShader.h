#pragma once

#include <Cabrankengine/Renderer/Shader.h>
#include <cstdint>
#include <glm/glm.hpp>

namespace cabrankengine {

	class OpenGLShader : public Shader {
		public:
			OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
			virtual ~OpenGLShader();

			void bind() const override;
			void unbind() const override;

			void uploadUniformInt(const std::string& name, int value);

			void uploadUniformFloat1(const std::string& name, float value);
			void uploadUniformFloat2(const std::string& name, const glm::vec2& values);
			void uploadUniformFloat3(const std::string& name, const glm::vec3& values);
			void uploadUniformFloat4(const std::string& name, const glm::vec4& values);
			
			void uploadUniformMat3(const std::string& name, const glm::mat3& matrix);
			void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		private:
			uint32_t m_RendererId;
	};
}